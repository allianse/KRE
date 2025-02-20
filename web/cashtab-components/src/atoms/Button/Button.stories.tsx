// @flow

import React from 'react';

import { storiesOf } from '@storybook/react';

import Button from './Button';
import Text from '../Text';

const ButtonText = 'Cashtab Pay';

storiesOf('Button', module)
    .add(
        'default',
        () => (
            <Button step={'fresh'}>
                <Text>{ButtonText}</Text>
            </Button>
        ),
        {
            notes: 'Button is a stateful controlled component which is the primary visual indicator for the badger payment process',
        },
    )
    .add(
        'payment pending',
        () => (
            <Button step={'pending'}>
                <Text>{ButtonText}</Text>
            </Button>
        ),
        {
            notes: 'Awaiting a confirmation or cancellation of Badger popup',
        },
    )
    .add(
        'payment complete',
        () => (
            <Button step={'complete'}>
                <Text>{ButtonText}</Text>
            </Button>
        ),
        {
            notes: 'Payment received, at least on the front-end',
        },
    )
    .add(
        'install prompt',
        () => (
            <Button step={'install'}>
                <Text>{ButtonText}</Text>
            </Button>
        ),
        {
            notes: 'Cashtab extension not installed, prompt user to install Cashtab',
        },
    );
