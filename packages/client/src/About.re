let onSubmit = event => {
  ReactEvent.Form.preventDefault(event);
  Js.log(event);
};

[@react.component]
let make = () =>
  <div className="container text-center font-weight-bold mt-2">
    {"I'm just a guy." |> ReasonReact.string}
  </div>;
