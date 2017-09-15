let component = ReasonReact.statelessComponent "NotificationItem";

let make notification::(notification: Notifications.notification) ::onDelete _children => {
  ...component,
  render: fun _self =>
    <a className="no-underline underline-hover" href=(Notifications.githubUrl notification)>
      <article className="dt w-100 bb b--black-05 pb2 mt2 mh2">
        <div className="dtc w2 w3-ns v-mid">
          <img
            src=notification.repository.owner.avatar_url
            className="ba b--black-10 db br2 w2 w3-ns h2 h3-ns"
            alt="avatar"
          />
        </div>
        <div className="dtc v-mid pl3">
          <h1 className="f6 f5-ns fw6 lh-title purple mv0">
            (ReasonReact.stringToElement notification.subject.title)
          </h1>
          <h2 className="f6 fw4 mt0 mb0 light-purple">
            (ReasonReact.stringToElement notification.repository.full_name)
          </h2>
        </div>
        <div className="dtc v-mid">
          <div className="w-100 tr">
            <button
              className="f6 button-reset bg-white ba b--black-10 dim pointer pv1 black-60"
              onClick=(
                fun e => {
                  ReactEventRe.Mouse.preventDefault e;
                  onDelete notification
                }
              )>
              (ReasonReact.stringToElement "Mark as read")
            </button>
          </div>
        </div>
      </article>
    </a>
};
