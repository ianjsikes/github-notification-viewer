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
