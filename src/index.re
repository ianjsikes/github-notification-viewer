[%bs.raw {|require('tachyons/css/tachyons.css')|}];

[%bs.raw {|require('./index.css')|}];

external register_service_worker : unit => unit =
  "default" [@@bs.module "./registerServiceWorker"];

ReactDOMRe.renderToElementWithId <App /> "root";

register_service_worker ();
