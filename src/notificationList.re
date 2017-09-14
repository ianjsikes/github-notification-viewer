let component = ReasonReact.statelessComponent "NotificationList";

let make
  notifications::(notifications: array Notifications.notification)
  ::markAsRead
  _children => {
  ...component,
  render: fun _self =>
    <main className="mw6 center">
      (
        ReasonReact.arrayToElement (
          Array.map
            (
              fun (n: Notifications.notification) =>
                <NotificationItem key=n.id notification=n onDelete=markAsRead />
            )
            notifications
        )
      )
    </main>
};
