import React, { useEffect, useState } from 'react';
import {
	Page, PageHeader, PageSidebar, Dropdown, DropdownToggle, Avatar, DropdownGroup, DropdownItem,
	PageHeaderTools, Nav, NavList, NavItem, Button, Alert, AlertGroup, AlertVariant, AlertActionCloseButton, Split, SplitItem, Brand
} from '@patternfly/react-core';
import avatarImg from './avatar.svg';
import LoginModal from './LoginModal'
import RegistrationModal from './RegistrationModal'
import KeyIcon from '@patternfly/react-icons/dist/esm/icons/key-icon';
import logo from './fituska.png';
import bcrypt from 'bcryptjs';
import { useAction } from '../helpers/Hooks';

const Wrapper = ({ navIndex, children, userInfo, setUserInfo }) => {
	const [isNavOpen, setNavOpen] = useState(true);
	const [isDropdownOpen, setDropdownOpen] = useState(false);
	const [isLoginModalOpen, setLoginModalOpen] = useState(false);
	const [isRegistrationModalOpen, setRegistrationModalOpen] = useState(false);
	const [toastAlerts, setToastAlerts] = useState([]);

	const sendRegisterRequest = useAction('/register');
	const sendLoginRequest = useAction('/login');

	useEffect(() => {
		const savedInfo = localStorage.getItem('login');

		if (savedInfo != null) {
			try {
				const parsedInfo = JSON.parse(savedInfo);
				setUserInfo({ ...parsedInfo, loaded: true });
			}
			catch (e) {
				console.log("No info");
				setUserInfo({ loaded: true });
			}
		}
		else {
			setUserInfo({ loaded: true });
		}
	}, []);

	const addToastAlert = (variant, title, description) => {
		setToastAlerts([...toastAlerts, { title, variant, description, key: new Date().getTime() }]);
	};

	const removeToastAlert = key => {
		setToastAlerts(toastAlerts.filter(alert => alert.key !== key));
	};

	const registerUser = async (login, password, startYear, email, closeModal) => {
		const hashedPassword = await bcrypt.hash(password, 6);

		const successCallback = () => {
			addToastAlert(AlertVariant.success, 'Registrácia bola úspešná');
			closeModal();
		}

		const errorCallback = error => {
			addToastAlert(AlertVariant.danger, 'Registrácia nebola úspešná', error.response.data);
		}

		sendRegisterRequest({
			user: login,
			email,
			password: hashedPassword,
			startYear
		}, successCallback, errorCallback);
	}

	const loginUser = async (login, password, closeModal) => {
		const successCallback = receivedUserInfo => {
			addToastAlert(AlertVariant.success, 'Prihlásenie bolo úspešné');
			setUserInfo(receivedUserInfo.data);

			localStorage.setItem('login', JSON.stringify(receivedUserInfo.data));
			closeModal();
		}

		const errorCallback = error => {
			addToastAlert(AlertVariant.danger, 'Prihlásenie nebolo úspešné', error.response.data);
		}

		sendLoginRequest({
			user: login,
			password
		}, successCallback, errorCallback);
	}

	const userDropdownItems = [
		<DropdownGroup key="user-actions">
			<DropdownItem key="my-profile" onClick={() => {
				location.href = "/users/" + userInfo.username;
			}}>
				Môj profil
			</DropdownItem>
			<DropdownItem
				key="logout"
				onClick={() => {
					setUserInfo({});
					localStorage.removeItem('login');
					addToastAlert(AlertVariant.success, "Odhlásenie bolo úspešné")
					location.href = "/";
				}}>
				Odhlásiť sa
			</DropdownItem>
		</DropdownGroup>
	];

	const userDropdown = (
		<PageHeaderTools>
			{userInfo.username
				? <Dropdown
					onSelect={() => setDropdownOpen(!isDropdownOpen)}
					isOpen={isDropdownOpen}
					toggle={
						<DropdownToggle icon={<Avatar src={avatarImg} alt="Avatar" />} onToggle={() => setDropdownOpen(!isDropdownOpen)}>
							{userInfo.username}
						</DropdownToggle>
					}
					dropdownItems={userDropdownItems}
				/>
				: <Split hasGutter>
					<SplitItem>
						<Button variant="tertiary" onClick={() => setLoginModalOpen(true)}>Prihlásiť sa</Button>
					</SplitItem>
					<SplitItem>
						<Button variant="tertiary" onClick={() => setRegistrationModalOpen(true)}>Zaregistrovať sa</Button>
					</SplitItem>
				</Split>
			}
		</PageHeaderTools>
	);

	const header = (
		<PageHeader
			logo={<Brand src={logo} alt="Fituška v2" />}
			headerTools={userDropdown}
			showNavToggle
			isNavOpen={isNavOpen}
			onNavToggle={() => setNavOpen(!isNavOpen)}
		/>
	);

	const navigation = (
		<Nav>
			<NavList>
				<NavItem id="nav-link1" to="/" itemId={1} isActive={navIndex === 0}>
					Prehliadať kurzy
				</NavItem>
				{userInfo.username &&
					<NavItem id="nav-link2" to="/courses" itemId={0} isActive={navIndex === 1}>
						Moje kurzy
					</NavItem>
				}
				{(userInfo.role === "Moderator" || userInfo.role === "Admin") &&
					<NavItem id="nav-link3" to="/courseManagement" itemId={2} isActive={navIndex === 2}>
						Spravovať kurzy
						<KeyIcon style={{ marginLeft: 4 }} />
					</NavItem>
				}
				{userInfo.role === "Admin" &&
					<NavItem id="nav-link4" to="/userManagement" itemId={3} isActive={navIndex === 3}>
						Spravovať používateľov
						<KeyIcon style={{ marginLeft: 4 }} />
					</NavItem>
				}
				<NavItem id="nav-link5" to="/ladders" itemId={4} isActive={navIndex === 4}>
					Rebríček používateľov
				</NavItem>
				{(userInfo.role === "User" || userInfo.role === "Moderator" || userInfo.role === "Admin") &&
				<NavItem id="nav-link6" to="/mentored" itemId={5} isActive={navIndex === 5}>
					Spravovať mnou vytvorené kurzy
				</NavItem>
				}
			</NavList>
		</Nav>
	)

	const sidebar = (
		<PageSidebar
			nav={navigation}
			isNavOpen={isNavOpen}
		/*
		style={{
			backgroundColor: 'rgb(0, 169, 224)'
		}}
		*/
		/>
	)

	return (
		<Page header={header} sidebar={sidebar} style={{ height: '100vh' }}>
			<AlertGroup isToast isLiveRegion>
				{toastAlerts.map(({ key, variant, title, description }) => (
					<Alert
						variant={AlertVariant[variant]}
						title={title}
						timeout={5000}
						actionClose={
							<AlertActionCloseButton
								title={title}
								variantLabel={`${variant} alert`}
								onClose={() => removeToastAlert(key)}
							/>
						}
						key={key}
					>{description}</Alert>
				))}
			</AlertGroup>
			<LoginModal isOpen={isLoginModalOpen} setOpen={setLoginModalOpen} loginCallback={loginUser} />
			<RegistrationModal isOpen={isRegistrationModalOpen} setOpen={setRegistrationModalOpen} registerCallback={registerUser} />
			{React.cloneElement(children, { addToastAlert, userInfo })}
		</Page>
	)
};

export default Wrapper;
