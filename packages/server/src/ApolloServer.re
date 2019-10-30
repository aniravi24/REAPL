type schema = {. "schema": GraphqlSchema.schema};

type t;
type expressApp = {. "app": Express.App.t};

[@bs.module "apollo-server-express"] [@bs.new]
external make: schema => t = "ApolloServer";

[@bs.send]
external applyMiddleware: (t, expressApp) => unit = "applyMiddleware";
