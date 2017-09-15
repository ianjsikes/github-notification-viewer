/* Gets the stored github auth token from localStorage */
let getLocalToken () :option string => {
  let ls: string = [%bs.raw {| localStorage.getItem("githubToken") || "" |}];
  switch ls {
  | "" => None
  | _ => Some ls
  }
};

/* Stores a github auth token in localStorage */
let setLocalToken: string => unit = [%bs.raw {|
  function (token) {
    localStorage.setItem("githubToken", token);
  }
|}];

let deleteLocalToken: unit => unit = [%bs.raw {|
  function (token) {
    localStorage.removeItem("githubToken");
  }
|}];

/* Gets github temporary auth code from URL query parameters */
let getCodeFromUrl () :option string => {
  let match: string = [%bs.raw
  {|
    (window.location.href.match(/\?code=(.*)/) && window.location.href.match(/\?code=(.*)/)[1])
    ? window.location.href.match(/\?code=(.*)/)[1] : ''
  |}
  ];
  switch match {
  | "" => None
  | _ => Some match
  }
};

let getLocationOrigin: unit => string = [%bs.raw {|
  function () {
    return window.location.origin
  }  
|}];

/* Redirect window to github login url */
let redirectToAuth: string => string => unit = [%bs.raw {|
  function (clientId, redirectUri) {
    window.location = "https://github.com/login/oauth/authorize?scope=notifications&client_id=" + clientId + "&redirect_uri=" + redirectUri;
  }  
|}];

let decodeGatekeeperResponse json :string => Json.Decode.(field "token" string) json;

let requestToken code => {
  let reqInit = Bs_fetch.RequestInit.make
    method_::Bs_fetch.Post
    mode::Bs_fetch.CORS
    body::(Bs_fetch.BodyInit.make ("{ \"code\": \"" ^ code ^ "\" }"))
    headers::(Bs_fetch.HeadersInit.makeWithArray [|
      ("Content-Type", "text/plain")
    |])
    ();

  Bs_fetch.fetchWithInit "https://gh-notification-api.now.sh" reqInit |> Js.Promise.then_ Bs_fetch.Response.json |>
  Js.Promise.then_ (fun json => json |> decodeGatekeeperResponse |> Js.Promise.resolve);
};

let apiUrl endpoint => "https://api.github.com/" ^ endpoint;

let makeRequest token =>
  Bs_fetch.RequestInit.make
    headers::(Bs_fetch.HeadersInit.makeWithArray [|("Authorization", "token " ^ token)|]) ();

let makePatchRequest token =>
  Bs_fetch.RequestInit.make
    method_::Bs_fetch.Patch
    headers::(Bs_fetch.HeadersInit.makeWithArray [|("Authorization", "token " ^ token)|])
    ();

let user token => {
  let url = apiUrl "user";
  Bs_fetch.fetchWithInit url (makeRequest token) |> Js.Promise.then_ Bs_fetch.Response.text |>
  Js.Promise.then_ (fun jsonText => Js.Promise.resolve jsonText)
};

let notifications token =>
  Bs_fetch.fetchWithInit (apiUrl "notifications") (makeRequest token) |>
  Js.Promise.then_ Bs_fetch.Response.text |>
  Js.Promise.then_ Js.Promise.resolve;

let readNotification token id =>
  Bs_fetch.fetchWithInit (apiUrl "notifications/threads/" ^ id) (makePatchRequest token) |>
  Js.Promise.then_ (fun response => Js.Promise.resolve (Bs_fetch.Response.status response == 205));
