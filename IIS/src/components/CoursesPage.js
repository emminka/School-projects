import React, { useState } from 'react';
import { Card, CardBody, Button, ButtonVariant, AlertVariant, Split, SplitItem, Text, TextVariants, TextContent } from '@patternfly/react-core';
import NewCourseModal from './NewCourseModal';
import Table from './Table';
import { useFetch, useAction } from '../helpers/Hooks';
import PlusCircleIcon from '@patternfly/react-icons/dist/esm/icons/plus-circle-icon';

const CoursesPage = ({ userInfo, addToastAlert }) => {
    const COLUMNS = [
        { label: 'Názov kurzu', link: (content, row) => `/courses/${row[4]}` },
        { label: 'Počet otázok' },
        { label: 'Popis' },
        { label: 'Posledná aktivita', type: 'date' },
        { label: 'ID kurzu', type: 'hidden' }
    ];

    const [isNewCourseModalOpen, setNewCourseModalOpen] = useState(false);
    const [{ data, meta }, isLoading, refresh, { sortBy, onSort }, { page, perPage, onSetPage, onPerPageSelect }] = useFetch('/myCourses', userInfo);

    const createNewCourse = useAction('/createNewCourse', userInfo);
    const unsubscribeCourseRequest = useAction('/unsubscribeCourse', userInfo);

    const unsubscribeCourse = name => {
        const callback = () => {
            addToastAlert(AlertVariant.success, `Kurz "${name}" bol odhlásený`);
            refresh();
        }

        unsubscribeCourseRequest({course: name, username: userInfo.username}, callback);
    }

    const createNewCourseAction = courseInfo => {
        const callback = () => {
            addToastAlert(AlertVariant.success, 'Kurz bol úspešne založený', 'Po schválení moderátorom bude zverejnený');
            refresh();
        }

        createNewCourse(courseInfo, callback);
    }

    return (
        <Card>
            <NewCourseModal isOpen={isNewCourseModalOpen} setOpen={setNewCourseModalOpen} createCallback={courseInfo => createNewCourseAction(courseInfo)} />
            <CardBody>
                <Table
                    title={
                        <Split hasGutter>
                            <SplitItem>
                                <TextContent>
                                    <Text component={TextVariants.h1}>
                                        Moje prihlásené kurzy
                                    </Text>
                                </TextContent>
                            </SplitItem>
                            {userInfo.username &&
                                <SplitItem>
                                    <Button variant="primary" onClick={() => setNewCourseModalOpen(true)} icon={<PlusCircleIcon />}>Založiť nový kurz</Button>
                                </SplitItem>
                            }
                        </Split>
                    }
                    isLoading={isLoading}
                    columns={COLUMNS}
                    rows={data}
                    actions={userInfo.username && [{
                        label: 'Odhlásiť sa',
                        onClick: courseId => unsubscribeCourse(courseId),
                        buttonProps: {
                            variant: ButtonVariant.danger
                        }
                    }]}
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
    );
};

export default CoursesPage;