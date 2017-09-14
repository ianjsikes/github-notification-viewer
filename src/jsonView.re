external reactClass : ReasonReact.reactClass =
  "default" [@@bs.module "react-json-view"];

let make
  src::(src: option Js.Json.t)=?
  children =>
  ReasonReact.wrapJsForReason
    ::reactClass
    props::{
      "src": Js.Null_undefined.from_opt src
    }
    children;
