module Styles = {
  open Css;
  let header =
    style([
      textAlign(center),
      color(hex("343434")),
      fontWeight(normal),
      fontFamily("Ultra"),
      fontSize(`px(36)),
      lineHeight(`px(42)),
      textTransform(uppercase),
    ]);
};

module App = {
  [@react.component]
  let make = () =>
    <div className=Styles.header> {"REAPL" |> ReasonReact.string} </div>;
};

ReactDOMRe.renderToElementWithId(<App />, "index");