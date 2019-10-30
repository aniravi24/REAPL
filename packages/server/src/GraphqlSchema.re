type typeDefs = string;
type query = {. "dummy": unit => string};
type result = {. "result": string};
type code = {
  .
  "packages": string,
  "code": string,
};
type mutation = {. "runCode": (string, code) => result};
type resolvers = {
  .
  "Query": query,
  "Mutation": mutation,
};
type schema = {
  .
  "typeDefs": typeDefs,
  "resolvers": resolvers,
};
[@bs.module "apollo-server-express"]
external makeExecutableSchema: schema => schema = "makeExecutableSchema";

let typeDefs = {|
    type Query {
      dummy: String
    }
    type Mutation {
        runCode(packages: String!, code: String!): Result
    }
    type Result {
      result: String!
    }
|};

let resolvers = {
  "Query": {
    "dummy": () => "Dummy String",
  },
  "Mutation": {
    "runCode": (packages, code) => {
      /* For some reason, packages is undefined and code contains an object with the values */
      Js.log(packages);
      Js.log(code);
      let result = {"result": code##packages ++ code##code};
      result;
    },
  },
};

let schema = {"typeDefs": typeDefs, "resolvers": resolvers};
let graphqlSchema = makeExecutableSchema(schema);
