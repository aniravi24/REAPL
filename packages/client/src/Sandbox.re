/*
 type state = {greeting: string};

 type action =
   | UpdateResult(string);
 */

[@react.component]
let make = () =>
  <div className="container text-center font-weight-bold mt-2">
    {"REAPL" |> ReasonReact.string}
  </div>;

/*
 [@react.component]
 let make = () => {
   /*
    <GraphQLQueries.SayHelloQuery>

        ...{({result}) =>
          switch (result) {
          | Loading => <div> {"Loading" |> ReasonReact.string} </div>
          | Error(error) =>
            Js.log(error);
            <div> {"error" |> ReasonReact.string} </div>;
          | Data(data) =>
            Js.log(data);
            let world =
              Relude.Option.getOrElse("Data not present", data##hello);
            /*
             let results = f => {
               dispatch(UpdateResult(f));
             };
             results(world);
             */
            <div> {world |> ReasonReact.string} </div>;
          }
        }
      </GraphQLQueries.SayHelloQuery>;
      /*
       let (state, dispatch) =
         React.useReducer(
           (_, action) =>
             switch (action) {
             | UpdateResult(greeting) => {greeting: greeting}
             },
           {greeting: "No response yet"},
         );
         */
         */
 };
 */
