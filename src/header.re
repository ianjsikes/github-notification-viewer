let loginButton clientId redirectUri =>
  <a
    className="f6 dib white bg-animate hover-bg-white hover-black no-underline pv2 ph4 br-pill ba b--white-20"
    href=(
      "https://github.com/login/oauth/authorize?scope=notifications&client_id=" ^
      clientId ^ "&redirect_uri=" ^ redirectUri
    )>
    (ReasonReact.stringToElement "Log In")
  </a>;

let logoutButton onLogout =>
  <a
    className="f6 dib white bg-animate hover-bg-white hover-black no-underline pv2 ph4 br-pill ba b--white-20"
    onClick=onLogout>
    (ReasonReact.stringToElement "Log Out")
  </a>;

let component = ReasonReact.statelessComponent "Header";

let make ::loggedIn ::onLogout _children => {
  ...component,
  render: fun _self => {
    let btn =
      loggedIn ?
        logoutButton onLogout :
        loginButton "27778e7d924f0023ca79" "https://ianjsikes.github.io/github-notification-viewer";
    <nav
      className="flex justify-between bb b--white-10"
      style=(ReactDOMRe.Style.make backgroundColor::"#000000" ())>
      <div className="flex-grow pa3 flex items-center"> btn </div>
    </nav>
  }
};
