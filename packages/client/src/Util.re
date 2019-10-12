let saveSessionToStorage = (token, expiryDuration) => {
  Dom.Storage.(localStorage |> setItem("apollo-token", token));
  let expiryTime =
    Js.Date.now()
    /. 1000.0
    +. float_of_string(expiryDuration)
    |> Js.Float.toString;
  Dom.Storage.(localStorage |> setItem("apollo-token-exp", expiryTime));
};

let getTokenFromStorage = () =>
  Dom.Storage.(localStorage |> getItem("apollo-token"));

let removeSessionFromStorage = () => {
  Dom.Storage.(localStorage |> removeItem("apollo-token"));
  Dom.Storage.(localStorage |> removeItem("apollo-token-exp"));
};

let isSessionValid = () =>
  switch (Dom.Storage.(localStorage |> getItem("apollo-token-exp"))) {
  | None => false
  | Some(expiryTime) => float_of_string(expiryTime) > Js.Date.now() /. 1000.0
  };
