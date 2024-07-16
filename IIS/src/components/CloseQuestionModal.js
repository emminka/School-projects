import React, { useState } from 'react';
import { Form, FormGroup, TextInput, Button, Modal, ModalVariant, TextInputTypes, TextArea, Alert } from '@patternfly/react-core';

const CloseQuestionModal = ({ isOpen, setOpen, callback }) => {
  const [answerValue, setAnswerValue] = useState('');
	const [pointsValue, setPointsValue] = useState(0);

  const closeModal = () => {
		setAnswerValue('')
		setPointsValue('')
		setOpen(false)
	}

  return (
    <Modal
      variant={ModalVariant.small}
      title="Prihlásenie"
      isOpen={isOpen}
      onClose={closeModal}
      actions={[
        <Button key="confirm" variant="primary" isDisabled={answerValue === '' || !Number.isInteger(parseInt(pointsValue)) || pointsValue < 0} onClick={() => { callback(answerValue, pointsValue); closeModal(); }}>Uzavrieť otázku</Button>,
        <Button key="cancel" variant="link" onClick={closeModal}>Zrušiť</Button>
      ]}>
      <Form>
        <Alert variant="warning" title="Pred uzavretím otázky je potrebné označiť správne odpovede, po uzavretí to nebude možné" />
        <FormGroup label="Finálna odpoveď" isRequired>
          <TextArea
            isRequired
            id="login-password"
            name="login-password"
            value={answerValue}
						onChange={value => setAnswerValue(value)}
          />
        </FormGroup>
        <FormGroup label="Paušálne pridať body správnym odpovediam" isRequired>
          <TextInput
            isRequired
            id="login-password"
            name="login-password"
            value={pointsValue}
						onChange={value => setPointsValue(value)}
            type={TextInputTypes.number}
          />
        </FormGroup>
      </Form>
    </Modal>
  )
};

export default CloseQuestionModal;
