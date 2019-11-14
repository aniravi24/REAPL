[@bs.module "child_process"]
external spawnSync: (string, array(string)) => Node.Child_process.spawnResult =
  "spawnSync";

[@bs.module "child_process"]
external spawnSyncWithError: (string, array(string)) => unit = "spawnSync";

type typeDefs = string;
type query = {. "dummy": unit => string};
type result = {. "result": string};
type code = {
  .
  "bsconfig": string,
  "packages": string,
  "code": string,
};

type mutation = {. "runCode": (unit, code) => result};
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
        runCode(bsconfig: String!, packages: String!, code: String!): Result
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
    "runCode": (_, code) => {
      Js.log(code);
      switch (code##packages) {
      | "" => ()
      | _ =>
        Node.Child_process.execSync(
          "cd ../../../sandbox/ && yarn add " ++ code##packages,
          Node.Child_process.option(),
        )
        |> ignore
      };

      Node.Fs.writeFileAsUtf8Sync(
        "../../../sandbox/" ++ "./Code.re",
        code##code,
      );
      let bscSpawnResult =
        Node.Child_process.execSync(
          "cd ../../../sandbox/ && bsc Code.re | node",
          Node.Child_process.option(),
        );
      let finalResult =
        switch ({j|$bscSpawnResult|j}) {
        | "" =>
          let errorSpawnResult =
            try (
              Node.Child_process.execSync(
                "cd ../../../sandbox/ && bsc Code.re",
                Node.Child_process.option(),
              )
            ) {
            | Js.Exn.Error(e) =>
              Js.Exn.message(e)
              |> Js.Option.getWithDefault("Compilation Error")
            };
          {j|$errorSpawnResult|j};
        | _ => bscSpawnResult
        };
      let result = {"result": {j|$finalResult|j}};
      result;
    },
  },
};

let schema = {"typeDefs": typeDefs, "resolvers": resolvers};
let graphqlSchema = makeExecutableSchema(schema);
