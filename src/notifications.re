type owner = {
  id: int,
  login: string,
  owner_type: string,
  avatar_url: string
};

type subject = {
  title: string,
  url: string,
  latest_comment_url: string,
  subject_type: string
};

type repository = {
  full_name: string,
  html_url: string,
  id: int,
  name: string,
  private: bool,
  owner
};

type notification = {
  id: string,
  unread: bool,
  reason: string,
  updated_at: string,
  subject,
  repository
};

module Decode = {
  let owner json :owner =>
    Json.Decode.{
      id: field "id" int json,
      login: field "login" string json,
      owner_type: field "type" string json,
      avatar_url: field "avatar_url" string json
    };
  let subject json :subject =>
    Json.Decode.{
      title: field "title" string json,
      url: field "url" string json,
      latest_comment_url: field "latest_comment_url" string json,
      subject_type: field "type" string json
    };
  let repository json :repository =>
    Json.Decode.{
      owner: field "owner" owner json,
      full_name: field "full_name" string json,
      html_url: field "html_url" string json,
      id: field "id" int json,
      name: field "name" string json,
      private: field "private" bool json
    };
  let notification json :notification =>
    Json.Decode.{
      id: field "id" string json,
      unread: field "unread" bool json,
      reason: field "reason" string json,
      updated_at: field "updated_at" string json,
      subject: field "subject" subject json,
      repository: field "repository" repository json
    };
  /* Json.Decode.field */
  let notifications json :array notification => Json.Decode.(array notification) json;
};

let last (arr: array 'a) :'a => {
  let len = Array.length arr;
  arr.(len - 1)
};

let githubUrl notification :string => {
  let arr = Js.String.split "/" notification.subject.url;
  let l = last arr;
  let url =
    switch notification.subject.subject_type {
    | "Issue" => notification.repository.html_url ^ "/issues/" ^ l
    | "PullRequest" => notification.repository.html_url ^ "/pull/" ^ l
    | _ => ""
    };
  url
};

let fetchNotifications token =>
  Github.notifications token |>
  Js.Promise.then_ (
    fun jsonText => {
      Js.log (Js.Json.parseExn jsonText);
      jsonText |> Js.Json.parseExn |> Decode.notifications |> Js.Promise.resolve
    }
  );

let markAsRead token notification => Github.readNotification token notification.id;
