import React, { useState } from 'react';
import '@patternfly/react-core/dist/styles/base.css';
import {
  BrowserRouter as Router,
  Switch,
  Route
} from "react-router-dom";
import Wrapper from './Wrapper';
import CoursesPage from './CoursesPage';
import CourseDetails from './CourseDetails';
import QuestionPage from './QuestionPage';
import EmptyPage from './EmptyPage';
import CourseManagementPage from './CourseManagementPage';
import UserManagementPage from './UserManagementPage';
import ProfilePage from './ProfilePage';
import IndexPage from './IndexPage';
import LaddersPage from './LaddersPage';
import MentoredPage from './MentoredPage';

const App = () => {
  const [userInfo, setUserInfo] = useState({});

  return (
    <Router>
      <Switch>
        <Route exact path="/">
          <Wrapper navIndex={0} userInfo={userInfo} setUserInfo={setUserInfo}>
            <IndexPage />
          </Wrapper>
        </Route>
        <Route exact path="/courses">
          <Wrapper navIndex={1} userInfo={userInfo} setUserInfo={setUserInfo}>
            <CoursesPage />
          </Wrapper>
        </Route>
        <Route exact path="/courseManagement">
          <Wrapper navIndex={2} userInfo={userInfo} setUserInfo={setUserInfo}>
            <CourseManagementPage />
          </Wrapper>
        </Route>
        <Route exact path="/userManagement">
          <Wrapper navIndex={3} userInfo={userInfo} setUserInfo={setUserInfo}>
            <UserManagementPage />
          </Wrapper>
        </Route>
        <Route path="/users/:userId">
          {({ match }) => (
            <Wrapper userInfo={userInfo} setUserInfo={setUserInfo}>
              <ProfilePage match={match} />
            </Wrapper>
          )}
        </Route>
        <Route path="/courses/:courseId/:questionId">
          {({ match }) => (
            <Wrapper navIndex={0} userInfo={userInfo} setUserInfo={setUserInfo}>
              <QuestionPage match={match} />
            </Wrapper>
          )}
        </Route>
        <Route path="/courses/:courseId">
          {({ match }) => (
            <Wrapper navIndex={0} userInfo={userInfo} setUserInfo={setUserInfo}>
              <CourseDetails match={match} />
            </Wrapper>
          )}
        </Route>
        <Route path="/ladders/:courseId?">
          {({ match }) => (
            <Wrapper navIndex={4} userInfo={userInfo} setUserInfo={setUserInfo}>
              <LaddersPage match={match} />
            </Wrapper>
          )}
        </Route>
        <Route path="/mentored">
          {({ match }) => (
            <Wrapper navIndex={5} userInfo={userInfo} setUserInfo={setUserInfo}>
              <MentoredPage match={match} />
            </Wrapper>
          )}
        </Route>
        <Route path="/">
          <Wrapper userInfo={userInfo} setUserInfo={setUserInfo}>
            <EmptyPage />
          </Wrapper>
        </Route>
      </Switch>
    </Router>
  )
}

export default App;
