import React, { Fragment } from 'react';
import { ACHIEVEMENTS } from '../helpers/Constants';

const Username = ({ children, achievementId = null, link = true }) => (
    link ? (
        <a href={`/users/${children}`}>
            {children}
            <span title={ACHIEVEMENTS[achievementId]?.title} style={{ marginLeft: 4 }}>{ACHIEVEMENTS[achievementId]?.icon}</span>
        </a>
        ) : (
            <Fragment>
                {children}
                <span title={ACHIEVEMENTS[achievementId]?.title} style={{ marginLeft: 4 }}>{ACHIEVEMENTS[achievementId]?.icon}</span>
            </Fragment>
        )
);

export default Username;
