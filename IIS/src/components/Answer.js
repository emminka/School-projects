import React from 'react';
import { AlertVariant, Button, Card, CardBody, Split, SplitItem } from '@patternfly/react-core';
import RatingPanel from './RatingPanel';
import Username from './Username';
import { CheckCircleIcon, PlusCircleIcon } from '@patternfly/react-icons';
import { useAction } from '../helpers/Hooks';

const Answer = ({ author, authorBadge, date, content, points, isNested, isAccepted, imageId, answerId, respond, userInfo, refresh, addToastAlert, userVotes, refreshVotes, isUserRegistered, isUserMentor, isClosed }) => {
    const voteAction = useAction('/vote', userInfo);
    const acceptAnswerAction = useAction('/acceptAnswer', userInfo);

    const vote = value => {
        if (isClosed) {
            addToastAlert(AlertVariant.danger, `Nedá sa hlasovať pod uzavretou otázkou`);
            return;
        }

        if (userVotes.length >= 3 && !userVotes.some(item => item.answer_id === answerId)) {
            addToastAlert(AlertVariant.danger, `Nedá sa hlasovať za viac ako 3 odpovede`);
            return;
        }

        if (userVotes.some(item => item.answer_id === answerId && item.vote_voted === value)) {
            return;
        }

        const callback = () => {
            addToastAlert(AlertVariant.success, `Hlasovanie bolo úspešné`);
            refresh();
            refreshVotes();
        }

        voteAction({ value, username: userInfo.username, answerId }, callback);
    }

    const accept = (answerId, points, author) => {
        const callback = () => {
            addToastAlert(AlertVariant.success, `Odpoveď bola označená ako správna`);
            refresh();
        }

        acceptAnswerAction({ answerId, points, author, mentor: userInfo.username }, callback);
    }

    return (
        <Card style={{ marginBottom: 16, ...isNested && { marginLeft: 50 }, ...isAccepted && { backgroundColor: '#b8d6c1' } }}>
            <CardBody>
                <Split style={{ width: "100%" }}>
                    <SplitItem>
                        <RatingPanel isUserRegistered={isUserRegistered} answerId={answerId} points={points} userVotes={userVotes} voteUp={() => vote("upvote")} voteDown={() => vote("downvote")}/>
                    </SplitItem>
                    <SplitItem style={{ fontSize: 20, width: "100%" }}>
                        <Split>
                            <SplitItem>
                                <Username achievementId={authorBadge}>{author}</Username>
                                <span style={{ color: 'gray' }}> • {new Date(date).toLocaleString("cs-CZ")}</span> <br />
                                <p style={{ marginTop: 8, fontSize: 16 }}>{content}</p>
                            </SplitItem>
                            <SplitItem isFilled />
                            <SplitItem>
                                {!isNested && isUserRegistered && !isUserMentor && !isClosed && <Button variant="primary" onClick={() => respond({ id: answerId, author })} icon={<PlusCircleIcon />}>Odpovedať</Button>}
                                {!isNested && isUserMentor && !isClosed && !isAccepted && <Button variant="primary" onClick={() => accept(answerId, points, author)} icon={<CheckCircleIcon />}>Označiť ako správnu</Button>}
                            </SplitItem>
                        </Split>
                    </SplitItem>
                </Split>
                {
                    (imageId !== null) &&
                    <img style={{ maxHeight: 400, maxWidth: "80%" }} src={`data:image/png;base64,${imageId}`} />
                }
            </CardBody>
        </Card>
    )
};

export default Answer;
