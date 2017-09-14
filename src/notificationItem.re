let component = ReasonReact.statelessComponent "NotificationItem";

let make notification::(notification: Notifications.notification) ::onDelete _children => {
  ...component,
  render: fun _self =>
    <div className="notification-item">
      <a href=(Notifications.githubUrl notification)>
        (ReasonReact.stringToElement notification.subject.title)
      </a>
      <button onClick=(fun _ => onDelete notification)>
        (ReasonReact.stringToElement "Delete")
      </button>
    </div>
};
