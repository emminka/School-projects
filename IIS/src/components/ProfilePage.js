import React from 'react';
import { Card, CardBody, Text, TextContent, TextVariants, Split, SplitItem, Avatar, Bullseye, Button, AlertVariant } from '@patternfly/react-core';
import { useAction, useFetch } from '../helpers/Hooks';
import avatarImg from './avatar.svg';
import Table from './Table';
import EmptyPage from './EmptyPage';
import { ACHIEVEMENTS } from '../helpers/Constants';
import Username from './Username';

const ProfilePage = ({ match, userInfo, addToastAlert }) => {
    const COLUMNS = [
        { label: 'Názov kurzu', link: (content, row) => `/courses/${row[4]}` },
        { label: 'Počet otázok' },
        { label: 'Popis' },
        { label: 'Posledná aktivita', type: 'date' },
        { label: 'ID kurzu', type: 'hidden' }
    ];

    const [profileInfo, isProfileInfoLoading, refreshProfileInfo] = useFetch(`/userInfo?user=${match.params.userId}`, userInfo)
    const [achievements, areAchievementsLoading] = useFetch(`/achievements?user=${match.params.userId}`, userInfo)

    const [{ data, meta }, isLoading, refresh, { sortBy, onSort }, { page, perPage, onSetPage, onPerPageSelect }] = useFetch(`/managedCourses?user=${match.params.userId}`, userInfo);

    const setPreferredBadgeAction = useAction('/setPreferredBadge', userInfo);

    const setPreferredBadge = badge => {
        const callback = () => {
            addToastAlert(AlertVariant.success, "Odznak pri mene bol úspešne zmenený");
            refreshProfileInfo();
        }

        setPreferredBadgeAction({ badge, username: userInfo.username }, callback);
    }

    const achievementResolver = () => {
        let received = [];

        if (!areAchievementsLoading) {
            parseInt(achievements.courses_registered) >= 1 ? received.push(true) : received.push(false);
            parseInt(achievements.courses_registered) >= 10 ? received.push(true) : received.push(false);
            parseInt(achievements.questions) >= 1 ? received.push(true) : received.push(false);
            parseInt(achievements.questions) >= 10 ? received.push(true) : received.push(false);
            parseInt(achievements.points) >= 10 ? received.push(true) : received.push(false);
            parseInt(achievements.points) >= 100 ? received.push(true) : received.push(false);
            parseInt(achievements.points) >= 1000 ? received.push(true) : received.push(false);
            parseInt(achievements.accepted) >= 1 ? received.push(true) : received.push(false);
            parseInt(achievements.accepted) >= 10 ? received.push(true) : received.push(false);
            parseInt(achievements.courses_created) >= 1 ? received.push(true) : received.push(false);
        }

        return received;
    }

    return (!isProfileInfoLoading && profileInfo.length === 0) ? <EmptyPage text="Používateľ neexistuje" /> : (
        <Card>
            <CardBody>
                <Bullseye style={{ height: 450 }}>
                    <Split hasGutter style={{ marginBottom: 32 }}>
                        <SplitItem>
                            <Avatar src={avatarImg} alt="Avatar" style={{ width: 300, height: 300 }} />
                        </SplitItem>
                        <SplitItem>
                            <TextContent>
                                <Text component={TextVariants.h1}>
                                    <Username achievementId={profileInfo?.preferred_badge} link={false}>{match.params.userId}</Username>
                                </Text>
                                <Split hasGutter>
                                    <SplitItem>
                                        {isProfileInfoLoading || <Text>
                                            <b>Začiatok štúdia</b><br />
                                            {profileInfo.study_start}<br />
                                            <b>Body</b><br />
                                            {profileInfo.points}<br />
                                            <b>Počet otázok</b><br />
                                            {profileInfo.question_count}<br />
                                            <b>Počet odpovedí</b><br />
                                            {profileInfo.answer_count}<br />
                                            <b>Rola</b><br />
                                            {profileInfo.role}<br />
                                            <b>Prvé prihlásenie</b><br />
                                            {new Date(profileInfo.first_login).toLocaleString("cs-CZ")}<br />
                                            <b>Posledné prihlásenie</b><br />
                                            {new Date(profileInfo.most_recent_login).toLocaleString("cs-CZ")}<br />
                                        </Text>
                                        }
                                    </SplitItem>
                                    <SplitItem>
                                        <b>Dosiahnuté úspechy</b>
                                        <table border="0">
                                            {!areAchievementsLoading && ACHIEVEMENTS.map((achievement, index) => achievementResolver()[index] ? (
                                                <tr>
                                                    <td style={{ fontSize: 24, textAlign: 'center' }}>{achievement.icon}</td>
                                                    <td>{achievement.title}</td>
                                                    {userInfo.username === match.params.userId && <td><Button onClick={() => setPreferredBadge(index)}>Zobrazovať odznak pri mene</Button></td>}
                                                </tr>
                                            ) : (
                                                <tr>
                                                    <td style={{ fontSize: 24, color: 'transparent', textShadow: '0 0 0 gray', textAlign: 'center' }}>{achievement.icon}</td>
                                                    <td style={{ color: 'gray' }}>{achievement.title}</td>
                                                    {userInfo.username === match.params.userId && <td></td>}
                                                </tr>
                                            ))}
                                        </table>
                                    </SplitItem>
                                </Split>
                            </TextContent>
                        </SplitItem>
                    </Split>
                </Bullseye>
                <Table
                    title={
                        <TextContent>
                            <Text component={TextVariants.h1}>
                                Schválené kurzy založené používateľom
                            </Text>
                        </TextContent>
                    }
                    isLoading={isLoading}
                    columns={COLUMNS}
                    rows={data}
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

export default ProfilePage;
