import React, { useState } from 'react';
import {
    Card, CardBody, TextContent, Text, TextVariants, ButtonVariant, AlertVariant, Split, SplitItem, Button, Modal, ModalVariant, Form, FormGroup, TextInput
} from '@patternfly/react-core';
import { useAction, useFetch } from '../helpers/Hooks';
import Table from './Table';
import Username from './Username';
import EmptyPage from './EmptyPage';
import PlusCircleIcon from '@patternfly/react-icons/dist/esm/icons/plus-circle-icon';

const MentoredPage = ({ userInfo, match, addToastAlert }) => {
    const COLUMNS_USERS = [
        { label: 'Používateľské meno', link: user => `/users/${user}` },
        { label: 'Email' },
        { label: 'Kurz' }
    ];
    
    const COLUMNS_CATEGORIES = [
        { label: 'Názov kategórie' },
        { label: 'Počet otázok' },
        { label: 'Počet odpovedí' }
    ];

    const [{ data, meta }, isLoading, refresh, { sortBy, onSort }, { page, perPage, onSetPage, onPerPageSelect }] = useFetch('/getWaiting', userInfo, sort => sort + 1, undefined, 0, 'desc', { mentor: userInfo.username });
    const [isUserMentor] = useFetch(`/isMentor?user=${userInfo.username}`, userInfo);

    const acceptAction = useAction('/acceptUser', userInfo);

    const [{ data: categoriesData, meta: categoriesMeta }, isLoadingCategories, refreshCategories, categoriesSorting, categoriesPagination] = useFetch('/categories', userInfo);
    const [isModalOpen, setModalOpen] = useState(false);
    const [modalCategory, setModalCategory] = useState('');

    const removeCategoryRequest = useAction('/removeCategory', userInfo);
    const addCategoryRequest = useAction('/addCategory', userInfo);

    const accept = (username, courseName, verdict) => {
        const acceptCallback = () => {
            addToastAlert(AlertVariant.success, `Používateľ bol prijatý do kurzu`);
            refresh();
        }

        const denyCallback = () => {
            addToastAlert(AlertVariant.success, `Používateľ bol odmietnutý z kurzu`);
            refresh();
        }

        if (verdict === "yes") {
            acceptAction({ userToAccept: username, courseName, verdict }, acceptCallback);
        }
        else {
            acceptAction({ userToAccept: username, courseName, verdict }, denyCallback);
        }
    }

    const addCategory = name => {
        const callback = () => {
            addToastAlert(AlertVariant.success, `Kategória "${name}" bola pridaná`);
            refreshCategories();
        }

        addCategoryRequest({ category: name }, callback);
    }

    const removeCategory = name => {
        const callback = () => {
            addToastAlert(AlertVariant.success, `Kategória "${name}" bola odstránená`);
            refreshCategories();
        }

        removeCategoryRequest({ category: name }, callback);
    }

    const newCategoryModal = (
        <Modal
            variant={ModalVariant.small}
            title="Vytvoriť novú kategóriu"
            isOpen={isModalOpen}
            onClose={() => setModalOpen(false)}
            actions={[
                <Button key="confirm" variant="primary" isDisabled={modalCategory?.length === 0} onClick={() => { addCategory(modalCategory); setModalOpen(false); }}>Vytvoriť</Button>,
                <Button key="cancel" variant="link" onClick={() => setModalOpen(false)}>Zrušiť</Button>
            ]}>
            <Form>
                <FormGroup label="Názov kategórie" isRequired>
                    <TextInput
                        isRequired
                        id="new-category"
                        name="new-category"
                        value={modalCategory}
                        onChange={value => setModalCategory(value)}
                    />
                </FormGroup>
            </Form>
        </Modal>
    )

    return !isUserMentor ? <EmptyPage text="Žiadne kurzy nespravujete" /> : (
        <Card>
            {newCategoryModal}
            <CardBody>
                <Table
                    title={
                        <TextContent>
                            <Text component={TextVariants.h1}>
                                Používatelia čakajúci na schválenie
                            </Text>
                        </TextContent>
                    }
                    rows={data?.map(([preferredBadge, username, ...rest]) => [<Username achievementId={preferredBadge} key={username}>{username}</Username>, ...rest])}
                    columns={COLUMNS_USERS}
                    actions={userInfo.username && [{
                        label: 'Prijať',
                        onClick: (username, email, courseName) => accept(username.key, courseName, "yes"),
                        buttonProps: {
                            variant: ButtonVariant.primary,
                            style: {
                                backgroundColor: '#3E8635',
                                marginRight: 16
                            }
                        }
                    },
                    {
                        label: 'Odmietnuť',
                        onClick: (username, email, courseName) => accept(username.key, courseName, "no"),
                        buttonProps: {
                            variant: ButtonVariant.danger
                        }
                    }]}
                    isLoading={isLoading}
                    sortBy={sortBy}
                    onSort={onSort}
                    page={page}
                    perPage={perPage}
                    itemCount={meta?.itemCount}
                    onSetPage={onSetPage}
                    onPerPageSelect={onPerPageSelect}
                />
                <hr style={{ marginTop: 32, marginBottom: 32 }} />
                <Table
                    title={
                        <Split hasGutter>
                            <SplitItem>
                                <TextContent>
                                    <Text component={TextVariants.h1}>
                                        Kategórie
                                    </Text>
                                </TextContent>
                            </SplitItem>
                            <SplitItem>
                                <Button variant="primary" onClick={() => setModalOpen(true)} icon={<PlusCircleIcon />}>Vytvoriť novú kategóriu</Button>
                            </SplitItem>
                        </Split>
                    }
                    isLoading={isLoadingCategories}
                    columns={COLUMNS_CATEGORIES}
                    rows={categoriesData}
                    actions={[{
                        label: 'Odstrániť',
                        onClick: categoryId => removeCategory(categoryId),
                        buttonProps: {
                            variant: ButtonVariant.danger
                        }
                    }]}
                    itemCount={categoriesMeta?.itemCount}
                    {...categoriesSorting}
                    {...categoriesPagination}
                />
            </CardBody>
        </Card>
    )
};

export default MentoredPage;
