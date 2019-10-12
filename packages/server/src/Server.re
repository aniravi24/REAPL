[@bs.module] external morgan: string => Express.Middleware.t = "morgan";
let makeSuccessJson = () =>
  Json.Encode.object_([("success", Json.Encode.bool(true))]);

let app = Express.express();
Express.App.disable(app, ~name="x-powered-by");

Express.App.use(app, Express.Middleware.json());
Express.App.use(app, Express.Middleware.urlencoded());
Express.App.use(app, morgan("dev"));
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
  ~path="/",
  Express.Middleware.from((_, _, res) =>
    Express.Response.sendFile("index.html", {"root": "../client/dist/"}, res)
  ),
);

let server = Express.App.listen(app, ~port=8000, ());
