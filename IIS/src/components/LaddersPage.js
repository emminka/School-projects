import React from 'react';
import {
    Card, CardBody, TextContent, Text, TextVariants, Breadcrumb, BreadcrumbItem
} from '@patternfly/react-core';
import { useFetch } from '../helpers/Hooks';
import Table from './Table';
import Username from './Username';

const LaddersPage = ({ userInfo, match }) => {
    const COLUMNS = [
        { label: 'Používateľské meno', link: user => `/users/${user}` },
        ...match?.params?.courseId ? [] : [{ label: 'Body' }],
        { label: 'Počet otázok' },
        { label: 'Počet odpovedí' }
    ];

    const [courseName] = useFetch(`/getCourseName?courseId=${match.params.courseId}`, userInfo);
    const [{ data, meta }, isLoading, refresh, { sortBy, onSort }, { page, perPage, onSetPage, onPerPageSelect }] = useFetch('/getLadders' + (match?.params?.courseId ? '?course=' + match?.params?.courseId : ''), userInfo,  sort => sort + 1, undefined, 1, 'desc', { courseId: match?.params?.courseId });

    return (
        <Card>
            <CardBody>
                {
                    match?.params?.courseId && <Breadcrumb style={{ marginBottom: 16 }}>
                        <BreadcrumbItem to="/courses">Kurzy</BreadcrumbItem>
                        <BreadcrumbItem to={`/courses/${match?.params?.courseId}`}>{courseName}</BreadcrumbItem>
                        <BreadcrumbItem isActive>
                            Rebríček
                        </BreadcrumbItem>
                    </Breadcrumb>
                }
                <Table
                    title={
                        <TextContent>
                            <Text component={TextVariants.h1}>
                                Rebríček používateľov {courseName && ("pre kurz " + courseName)}
                            </Text>
                        </TextContent>
                    }
                    rows={data?.map(([preferredBadge, username, ...rest]) => [<Username achievementId={preferredBadge} key={username}>{username}</Username>, ...rest])}
                    columns={COLUMNS}
                    isLoading={isLoading}
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
    )
};

export default LaddersPage;
