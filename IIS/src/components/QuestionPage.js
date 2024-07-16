import React, { useState, Fragment } from 'react';
import {
    Card, CardBody, Breadcrumb, BreadcrumbItem, FileUpload, TextContent, Text, TextVariants, Pagination, TextArea, Form, AlertVariant, FormGroup, Button, Split, SplitItem, Label
} from '@patternfly/react-core';
import Answer from './Answer';
import { useAction, useActionWithFile, useFetch } from '../helpers/Hooks';
import TableEmptyState from './TableEmptyState';
import { LockIcon, QuestionCircleIcon } from '@patternfly/react-icons';
import Username from './Username';
import CloseQuestionModal from './CloseQuestionModal';
import EmptyPage from './EmptyPage';

const QuestionPage = ({ match, addToastAlert, userInfo }) => {
    const [questionDetail, isQuestionDetailLoading, refreshQuestionDetail] = useFetch(`/questionDetail?question=${match.params.questionId}`, userInfo);
    const [{ data: answers, meta }, isLoading, refresh] = useFetch(`/answers?question=${match.params.questionId}`, userInfo);
    const [isUserRegistered] = useFetch(`/isRegistered?courseId=${match.params.courseId}&user=${userInfo.username}`, userInfo);
    const [isUserMentor] = useFetch(`/isMentor?courseId=${match.params.courseId}&user=${userInfo.username}`, userInfo);
    const [hasUserAnswered, isHasUserAnsweredLoading, refreshHasUserAnswered] = useFetch(`/hasUserAnswered?question=${match.params.questionId}&user=${userInfo.username}`, userInfo);
    const [userVotes, _, refreshVotes] = useFetch(`/getUserVotesForQuestion?user=${userInfo.username}&question=${match.params.questionId}`, userInfo);
    const [courseName] = useFetch(`/getCourseName?courseId=${match.params.courseId}`, userInfo);

    const [newAnswer, setAnswer] = useState('');
    const [respondingId, setRespondingId] = useState(null);
    const [isModalOpen, setModalOpen] = useState(false);

    const [fileData, setFileData] = useState(null);
    const [fileName, setFileName] = useState('');

    const addAnswerRequest = useActionWithFile('/addAnswer', userInfo);
    const closeQuestionAction = useAction('/closeQuestion', userInfo);

    const getSortedAnswers = () => {
        const sortedAnswers = [...answers];
        sortedAnswers?.sort((a, b) => Date.parse(a[1]) - Date.parse(b[1]));
        const answersList = sortedAnswers?.filter(answer => !answer[3]).map(item => [item]);
        const reactionList = sortedAnswers?.filter(reaction => reaction[3])

        reactionList?.forEach(reaction => answersList?.find(answer => answer[0][7] === reaction[3]).push(reaction))

        return answersList?.flat();
    };

    const addAnswer = (text, file, responseId = undefined) => {
        const callback = () => {
            addToastAlert(AlertVariant.success, `Odpoveď bola pridaná`);
            refresh();
            refreshHasUserAnswered();

            setAnswer('')
            setRespondingId(null)
            setFileData(null)
            setFileName('')
        }

        addAnswerRequest({ text, authorName: userInfo.username, questionName: match.params.questionId, responseId }, file, callback);
    }

    const closeQuestion = (answer, points) => {
        const callback = () => {
            addToastAlert(AlertVariant.success, `Otázka bola uzavretá`);
            refresh();
            refreshQuestionDetail();
        }

        closeQuestionAction({ answer, points, user: userInfo.username, questionName: match.params.questionId }, callback);
    }

    return !isQuestionDetailLoading && !questionDetail ? <EmptyPage /> : (
        <Card>
            <CloseQuestionModal isOpen={isModalOpen} setOpen={setModalOpen} callback={closeQuestion} />
            <CardBody>
                <Breadcrumb style={{ marginBottom: 15 }}>
                    <BreadcrumbItem to="/courses">Kurzy</BreadcrumbItem>
                    <BreadcrumbItem to={`/courses/${match.params.courseId}`}>{courseName}</BreadcrumbItem>
                    <BreadcrumbItem isActive>
                        {questionDetail?.title}
                    </BreadcrumbItem>
                </Breadcrumb>
                <TextContent>
                    <Split>
                        <SplitItem>
                            <Text component={TextVariants.h1} style={{ marginBottom: 8 }}>
                                {questionDetail?.title} {questionDetail?.is_closed && <Label color='red' icon={<LockIcon />} style={{ marginLeft: 8, verticalAlign: 4 }}>Uzavretá</Label>}
                            </Text>
                        </SplitItem>
                        <SplitItem isFilled />
                        <SplitItem>
                            {userInfo.username && !questionDetail.is_closed && isUserMentor && <Button key="close" variant="primary" onClick={() => setModalOpen(true)}>Uzavrieť otázku</Button>}
                        </SplitItem>
                    </Split>
                    <Text component={TextVariants.p} style={{ marginBottom: 8 }}>
                        Autor: <Username achievementId={questionDetail?.preferred_badge}>{questionDetail?.author}</Username>
                    </Text>
                    <Text component={TextVariants.p} style={{ marginBottom: 8 }}>
                        {questionDetail?.description}
                    </Text>
                </TextContent>
                {answers?.length > 0 ?
                    <React.Fragment>
                        {getSortedAnswers()?.map(answer =>
                            <Answer
                                author={answer[0]}
                                date={answer[1]}
                                content={answer[2]}
                                isNested={answer[3]}
                                points={answer[4]}
                                isAccepted={answer[5]}
                                imageId={answer[6]}
                                answerId={answer[7]}
                                authorBadge={answer[8]}
                                respond={setRespondingId}
                                userInfo={userInfo}
                                refresh={refresh}
                                addToastAlert={addToastAlert}
                                userVotes={userVotes}
                                refreshVotes={refreshVotes}
                                isUserRegistered={isUserRegistered}
                                isUserMentor={isUserMentor}
                                isClosed={questionDetail.is_closed}
                            />
                        )}
                    </React.Fragment>
                    : <TableEmptyState icon={QuestionCircleIcon} >Zatiaľ žiadne odpovede</TableEmptyState>
                }
                {userInfo.username && !questionDetail.is_closed && isUserRegistered && !isUserMentor && questionDetail?.author !== userInfo.username && (!hasUserAnswered || respondingId != null) &&
                    <Form>
                        <FormGroup label="Pridať odpoveď" isRequired>
                            {respondingId !== null && (
                                <Fragment>
                                    {`Reagujete na odpoveď používateľa ${respondingId.author}`}
                                    <Button variant="primary" style={{ marginLeft: 16, marginBottom: 8 }} onClick={() => setRespondingId(null)}>Zrušiť</Button>
                                </Fragment>
                            )}
                            <TextArea
                                isRequired
                                id="new-answer"
                                name="new-answer"
                                resizeOrientation='vertical'
                                value={newAnswer}
                                onChange={value => setAnswer(value)}
                                style={{ height: 100 }}
                            />
                            <FileUpload
                                id="simple-file"
                                value={fileData}
                                filename={fileName}
                                filenamePlaceholder="Žiadny obrázok nepriložený"
                                browseButtonText="Nahrať"
                                clearButtonText="Vymazať"
                                onFileInputChange={(event, newFileData) => { setFileData(newFileData); setFileName(newFileData.name) }}
                                onClearClick={event => { setFileName(''); setFileData(null) }}
                            />
                        </FormGroup>
                        <Split>
                            <SplitItem isFilled />
                            <SplitItem>
                                <Button key="confirm" variant="primary" isDisabled={newAnswer.length === 0} onClick={() => addAnswer(newAnswer, fileData, respondingId?.id)} style={{ width: 200 }}>Odpovedať</Button>
                            </SplitItem>
                        </Split>
                    </Form>
                }
            </CardBody>
        </Card >
    )
}

export default QuestionPage;
