module App = {
  [@react.component]
  let make = () => <div> {"REAPL" |> React.string} </div>;
};

ReactDOMRe.renderToElementWithId(<App />, "index");