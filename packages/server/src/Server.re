[@bs.module] external morgan: string => Express.Middleware.t = "morgan";
[@bs.module] external helmet: unit => Express.Middleware.t = "helmet";

let app = Express.express();
Express.App.use(app, Express.Middleware.json());
Express.App.use(app, Express.Middleware.urlencoded());
Express.App.use(app, morgan("dev"));
Express.App.use(app, helmet());

let apolloServerInstance =
  ApolloServer.make({schema: GraphqlSchema.graphqlSchema});
ApolloServer.applyMiddleware(apolloServerInstance, {app: app});

Express.App.useOnPath(
  app,
  ~path="/",
  {
    let options = Express.Static.defaultOptions();
    Express.Static.make("../client/dist/", options)
    |> Express.Static.asMiddleware;
  },
);
Express.App.get(
  app,
  ~path="*",
  Express.Middleware.from((_next, _req, res) =>
    Express.Response.sendFile("index.html", {"root": "../client/dist/"}, res)
  ),
);

let port =
  switch (Js.Nullable.toOption(ServerEnv.port)) {
  | Some(port) => int_of_string(port)
  | None => 8000
  };

let onListen = e =>
  switch (e) {
  | exception (Js.Exn.Error(e)) =>
    Js.log(e);
    Node.Process.exit(1);
  | _ => Js.log("Listening on http://localhost:" ++ string_of_int(port))
  };

let server = Express.App.listen(app, ~port, ~onListen, ());
