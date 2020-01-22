open Relude.Globals;

[@bs.module "child_process"]
external spawnSync: (string, array(string)) => Node.Child_process.spawnResult =
  "spawnSync";

[@bs.module "child_process"]
external spawnSyncWithError: (string, array(string)) => unit = "spawnSync";

type sources = {
  dir: string,
  subdirs: bool,
};
type packageSpecs = {
  [@bs.as "module"]
  module_: string,
  [@bs.as "in-source"]
  inSource: bool,
};

type warnings = {error: string};

type bsconfig = {
  name: string,
  version: string,
  sources,
  [@bs.as "package-specs"]
  packageSpecs,
  suffix: string,
  [@bs.as "bs-dependencies"]
  mutable bsDependencies: array(string),
  warnings,
  namespace: bool,
  refmt: int,
};

[@bs.scope "JSON"] [@bs.val]
external parseIntoBsconfig: string => bsconfig = "parse";

external bsconfigToJson: bsconfig => Js.Json.t = "%identity";

type typeDefs = string;
type query = {dummy: unit => string};
type result = {result: string};
type code = {
  bsconfig: string,
  packages: string,
  code: string,
};

type mutation = {runCode: (unit, code) => result};
type resolvers = {
  [@bs.as "Query"]
  query,
  [@bs.as "Mutation"]
  mutation,
};
type schema = {
  typeDefs,
  resolvers,
};
[@bs.module "apollo-server-express"]
external makeExecutableSchema: schema => schema = "makeExecutableSchema";

[@bs.get] external getStdOut: Js.Exn.t => option(Node.Buffer.t) = "stdout";

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
  query: {
    dummy: () => "Dummy String",
  },
  mutation: {
    runCode: (_, code) => {
      switch (code.bsconfig) {
      | "" => ()
      | _ =>
        let bsconfig =
          Node.Fs.readFileAsUtf8Sync("../../../sandbox/bsconfig.json");
        let parsedConfig = parseIntoBsconfig(bsconfig);
        parsedConfig.bsDependencies = Js.String.split(" ", code.bsconfig);

        let modifiedConfig = Js.Json.stringify(bsconfigToJson(parsedConfig));
        Node.Fs.writeFileAsUtf8Sync(
          "../../../sandbox/" ++ "./bsconfig.json",
          modifiedConfig,
        );
      };
      switch (code.packages) {
      | "" => ()
      | _ =>
        Node.Child_process.execSync(
          "cd ../../../sandbox/ && yarn add " ++ code.packages,
          Node.Child_process.option(),
        )
        |> ignore
      };

      Node.Fs.writeFileAsUtf8Sync(
        "../../../sandbox/src/" ++ "./Demo.re",
        code.code,
      );
      let bscSpawnResult =
        try (
          Node.Child_process.execSync(
            "cd ../../../sandbox/ && yarn build && node src/Demo.bs.js",
            Node.Child_process.option(),
          )
        ) {
        | Js.Exn.Error(e) =>
          e
          |> getStdOut
          |> Option.map(Node.Buffer.toString)
          |> Js.Option.getWithDefault("Compilation Error")
        };
      let bscSpawnResultString = {j|$bscSpawnResult|j};
      let finalResult =
        switch (Js.String.lastIndexOf("Building", bscSpawnResultString)) {
        | index =>
          let editOutput =
            Js.String.substring(
              ~from=index,
              ~to_=Js.String.length(bscSpawnResultString),
              bscSpawnResultString,
            );
          switch (Js.String.indexOf("\n", editOutput)) {
          | indexOutput =>
            Js.String.substring(
              ~from=indexOutput,
              ~to_=Js.String.length(editOutput),
              editOutput,
            )
          };
        };
      let result = {result: {j|$finalResult|j}};
      result;
    },
  },
};

let schema = {typeDefs, resolvers};
let graphqlSchema = makeExecutableSchema(schema);
