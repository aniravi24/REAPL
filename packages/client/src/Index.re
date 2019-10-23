let onSubmit = event => {
  ReactEvent.Form.preventDefault(event);
  Js.log(event);
};
module App = {
  [@react.component]
  let make = () => {
    let url = ReasonReactRouter.useUrl();
    let router =
      switch (url.path) {
      | ["about"] => <About />
      | _ => <Home />
      };
    <> router </>;
  };
};

ReactDOMRe.renderToElementWithId(<App />, "index");
