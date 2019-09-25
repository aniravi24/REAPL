module App = {
  [@react.component]
  let make = () => <div> {"REAPL" |> ReasonReact.string} </div>;
};

ReactDOMRe.renderToElementWithId(<App />, "index");