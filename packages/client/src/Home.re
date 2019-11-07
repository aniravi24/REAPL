open Relude.Globals;

type state = {
  packages: string,
  code: string,
  result: string,
};

type action =
  | ShowResult(string)
  | HandlePackagesChange(string)
  | HandleCodeChange(string);

[@react.component]
let make = () => {
  let (state, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | ShowResult(data) => {
            result: data,
            packages: state.packages,
            code: state.code,
          }
        | HandlePackagesChange(data) => {
            packages: data,
            result: state.result,
            code: state.code,
          }
        | HandleCodeChange(data) => {
            code: data,
            packages: state.packages,
            result: state.result,
          }
        },
      {packages: "", code: "", result: ""},
    );

  let runCodeMutation =
    GraphQLQueries.SendCode.make(
      ~packages=state.packages,
      ~code=state.code,
      (),
    );

  <GraphQLQueries.SendCodeMutation>
    ...{
         (mutate, {data}) => {
           let data =
             data
             |> Option.flatMap(d => d##runCode)
             |> Option.fold("No Data. Did you log the final return value?", d =>
                  d##result |> Js.String.make
                );
           <div className="container text-center font-weight-bold mt-2">
             {"REAPL" |> ReasonReact.string}
             <form
               className="form-group"
               onSubmit={
                 event => {
                   ReactEvent.Form.preventDefault(event);
                   mutate(~variables=runCodeMutation##variables, ()) |> ignore;
                 }
               }>
               <textarea
                 placeholder="Enter list of packages separated by line here"
                 className="form-control mt-2"
                 rows=5
                 value={state.packages}
                 onChange={
                   event =>
                     dispatch(
                       HandlePackagesChange(
                         ReactEvent.Form.target(event)##value,
                       ),
                     )
                 }
               />
               <textarea
                 placeholder="Enter code here"
                 className="form-control mt-5"
                 rows=10
                 value={state.code}
                 onChange={
                   event =>
                     dispatch(
                       HandleCodeChange(
                         ReactEvent.Form.target(event)##value,
                       ),
                     )
                 }
               />
               <button className="btn btn-primary mt-2">
                 {"Submit" |> ReasonReact.string}
               </button>
             </form>
             <div> {Js.String.make(data) |> ReasonReact.string} </div>
           </div>;
         }
       }
  </GraphQLQueries.SendCodeMutation>;
  /* <div> {res |> ReasonReact.string} </div> */
};
