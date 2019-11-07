[@react.component]
let make = () =>
  switch (ReasonReactRouter.useUrl()) {
  | {path: ["about"]} => <About />
  | {path: ["sandbox"]} => <Sandbox />
  | _ => <Home />
  };
