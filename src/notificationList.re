let component = ReasonReact.statelessComponent "NotificationList";

let make notifications::(notifications: array Notifications.notification) ::markAsRead _children => {
  ...component,
  render: fun _self =>
    switch (Array.length notifications) {
    | 0 =>
      <div className="mw6 center tc mv3">
        <h2 className="f5 f4-m f3-l fw2 black-50 mt0 lh-copy">
          (ReasonReact.stringToElement "Nothing to see here.")
        </h2>
      </div>
    | _ =>
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
    }
};
