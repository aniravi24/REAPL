[@bs.module "child_process"]
external spawnSync: (string, array(string)) => Node.Child_process.spawnResult =
  "spawnSync";

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
      Node.Fs.writeFileAsUtf8Sync("Code.re", code##code);
      let bscSpawnResult =
        Node.Child_process.execSync(
          "bsc Code.re | node",
          Node.Child_process.option(),
        );
      let finalResult =
        switch ({j|$bscSpawnResult|j}) {
        | "" =>
          Js.log("Syntax Error");
          let errorSpawnResult = spawnSync("bsc", [|"Code.re"|]);
          let stderr = Node.Child_process.readAs(errorSpawnResult)##stderr;
          {j|$stderr|j};
        | _ => bscSpawnResult
        };
      let result = {"result": {j|$finalResult|j}};
      result;
    },
  },
};

let schema = {"typeDefs": typeDefs, "resolvers": resolvers};
let graphqlSchema = makeExecutableSchema(schema);
