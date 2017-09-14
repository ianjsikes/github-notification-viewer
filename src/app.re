[%bs.raw {|require('./app.css')|}];

open Notifications;

type action =
  | LoadNotifications (array notification)
  | DeleteNotification notification;

type state = {notifications: option (array notification)};

let githubToken: string = [%bs.raw {| localStorage.getItem("githubToken") |}];

let component = ReasonReact.reducerComponent "App";

let make _children => {
  ...component,
  initialState: fun () => {notifications: None},
  didMount: fun self => {
    fetchNotifications githubToken |>
    Js.Promise.then_ (
      fun response =>
        {
          self.reduce
            (fun () => LoadNotifications (Js.Array.filter (fun n => n.unread) response)) ()
        } |> Js.Promise.resolve
    ) |> ignore;
    ReasonReact.NoUpdate
  },
  reducer: fun action state =>
    switch action {
    | LoadNotifications notifications => ReasonReact.Update {notifications: Some notifications}
    | DeleteNotification notification =>
      switch state.notifications {
      | Some notifications =>
        ReasonReact.Update {
          notifications: Some (Js.Array.filter (fun n => n.id != notification.id) notifications)
        }
      | None => ReasonReact.NoUpdate
      }
    },
  render: fun self => {
    let deleteNotification n => {
      markAsRead githubToken n |>
      Js.Promise.then_ (fun success => {
        switch success {
        | true => self.reduce (fun () => DeleteNotification n) ()
        | false => Js.log ("Could not delete notification " ^ n.id)
        };
        Js.Promise.resolve ();
      }) |>
      ignore;
    };

    let notificationList =
      switch self.state.notifications {
      | Some n => <NotificationList notifications=n markAsRead=deleteNotification/>
      | None => ReasonReact.stringToElement "Loading..."
      };
    <div className="App">
      <div className="App-header">
        <h1> (ReasonReact.stringToElement "Reason Projects") </h1>
      </div>
      notificationList
    </div>
  }
};
