module App = {
  [@react.component]
  let make = () =>
    <div className="container text-center font-weight-bold">
      {"REAPL" |> ReasonReact.string}
    </div>;
};

ReactDOMRe.renderToElementWithId(<App />, "index");