[@react.component]
let make = () => {
  let url = ReasonReactRouter.useUrl();
  let router =
    switch (url.path) {
    | ["about"] => <About />
    | ["sandbox"] => <Sandbox />
    | _ => <Home />
    };
  <> router </>;
};
