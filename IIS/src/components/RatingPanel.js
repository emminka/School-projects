import React from 'react';
import { Stack, StackItem } from '@patternfly/react-core';
import AngleUpIcon from '@patternfly/react-icons/dist/esm/icons/angle-up-icon';
import AngleDownIcon from '@patternfly/react-icons/dist/esm/icons/angle-down-icon';

const RatingPanel = ({ answerId, points, voteUp, voteDown, userVotes, isUserRegistered }) => (
    <Stack style={{ color: 'gray', marginRight: 24 }}>
        <StackItem>
            {isUserRegistered && <AngleUpIcon size='xl' onClick={voteUp} style={{ cursor: 'pointer', ...userVotes?.find(item => item.answer_id == answerId)?.vote_voted === 'upvote' && {color: 'green'} }}/>}
        </StackItem>
        <StackItem>
            <span style={{ display: 'inline-block', textAlign: 'center', width: 48, fontSize: 20 }}>
                {points}
            </span>
        </StackItem>
        <StackItem>
            {isUserRegistered && <AngleDownIcon size='xl' onClick={voteDown} style={{ cursor: 'pointer', ...userVotes?.find(item => item.answer_id == answerId)?.vote_voted === 'downvote' && {color: 'red'} }}/>}
        </StackItem>
    </Stack>
);

export default RatingPanel;
