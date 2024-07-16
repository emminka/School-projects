import React, { Fragment, useState } from 'react';
import { Card, CardBody, TextContent, Text, TextVariants, Breadcrumb, BreadcrumbItem, Label, Split, SplitItem, Button, AlertVariant, TextInput } from '@patternfly/react-core';
import { useAction, useFetch } from '../helpers/Hooks';
import Table from './Table';
import NewQuestionModal from './NewQuestionModal';
import Username from './Username';
import { TableIcon, PlusCircleIcon, LockIcon, SearchIcon } from '@patternfly/react-icons';
import EmptyPage from './EmptyPage';

const CourseDetails = ({ match, addToastAlert, userInfo }) => {
    const COLUMNS = [
        { label: 'Otázka', link: (question, row) => `/courses/${match.params.courseId}/${row[6]}` },
        { label: 'Kategória', fallback: 'Nezadaná' },
        { label: 'Autor', link: user => `/users/${user}` },
        { label: 'Počet odpovedí' },
        { label: 'Vytvorená', type: 'date' },
        { label: 'Posledná aktivita', type: 'date' },
        { label: 'ID otázky', type: 'hidden' }
    ];

    const [isModalOpen, setModalOpen] = useState(false);
    const [searchValue, setSearchValue] = useState('');

    const [{ data, meta }, isLoading, refresh, { sortBy, onSort }, { page, perPage, onSetPage, onPerPageSelect }] = useFetch(`/courseQuestions?course=${match.params.courseId}&search=${searchValue}`, userInfo, sort => sort + 2);
    const [isUserRegistered] = useFetch(`/isRegistered?courseId=${match.params.courseId}&user=${userInfo.username}`, userInfo);
    const [isUserMentor] = useFetch(`/isMentor?courseId=${match.params.courseId}&user=${userInfo.username}`, userInfo);
    const [courseName] = useFetch(`/getCourseName?courseId=${match.params.courseId}`, userInfo);
    const [isCourseApproved] = useFetch(`/isCourseApproved?courseId=${match.params.courseId}`, userInfo);

    const askQuestionRequest = useAction('/addQuestion', userInfo);

    const askQuestion = questionInfo => {
        const callback = () => {
            addToastAlert(AlertVariant.success, "Otázka bola položená");
            refresh();
        }

        askQuestionRequest({ ...questionInfo, courseName: match.params.courseId, authorName: userInfo.username }, callback);
    }

    return !isLoading && !courseName ? <EmptyPage /> : isCourseApproved ? (
        <Card>
            <NewQuestionModal userInfo={userInfo} isOpen={isModalOpen} setOpen={setModalOpen} createCallback={askQuestion} courseName={courseName} />
            <CardBody>
                <Breadcrumb style={{ marginBottom: 16 }}>
                    <BreadcrumbItem to="/courses">Kurzy</BreadcrumbItem>
                    <BreadcrumbItem isActive>
                        {courseName}
                    </BreadcrumbItem>
                </Breadcrumb>
                <Split hasGutter style={{ marginBottom: 16 }}>
                    <SplitItem>
                        <TextContent>
                            <Text component={TextVariants.h1}>
                                Kurz &quot;{courseName}&quot;
                            </Text>
                        </TextContent>
                    </SplitItem>
                    <SplitItem>
                        {userInfo.username && (isUserRegistered || isUserMentor) &&
                            <Button variant="primary" onClick={() => setModalOpen(true)} icon={<PlusCircleIcon />}>Položiť novú otázku</Button>
                        }
                    </SplitItem>
                    <SplitItem>
                        <Button variant="secondary" onClick={() => { location.href = '/ladders/' + match.params.courseId }} icon={<TableIcon />}>Rebríček pre kurz</Button>
                    </SplitItem>
                </Split>
                <Table
                    title={
                        <Split hasGutter>
                            <SplitItem>
                                <TextInput
                                    id="search"
                                    name="search"
                                    value={searchValue}
                                    onChange={value => setSearchValue(value)}
                                    placeholder={"Vyhľadávať v otázkach"}
                                />
                            </SplitItem>
                            <SplitItem>
                                <Button variant="primary" onClick={refresh} icon={<SearchIcon />}>Hľadať</Button>
                            </SplitItem>
                        </Split>
                    }
                    isLoading={isLoading}
                    columns={COLUMNS}
                    rows={data?.map(([locked, preferredBadge, questionId, category, author, ...rest]) => [
                        <Fragment key={questionId}>
                            {questionId}
                            {locked && <Label color='red' icon={<LockIcon />} style={{ marginLeft: 8 }}>Uzavretá</Label>}
                        </Fragment>, category, <Username achievementId={preferredBadge} key={questionId}>{author}</Username>, ...rest]
                    )}
                    sortBy={sortBy}
                    onSort={onSort}
                    page={page}
                    perPage={perPage}
                    itemCount={meta?.itemCount}
                    onSetPage={onSetPage}
                    onPerPageSelect={onPerPageSelect}
                />
            </CardBody>
        </Card>
    ) : <EmptyPage text="Tento kurz ešte nie je schválený"/>;
};

export default CourseDetails;
