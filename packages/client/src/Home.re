open Relude.Globals;

type state = {
  bsconfig: string,
  packages: string,
  code: string,
  result: string,
};

type action =
  | ShowResult(string)
  | HandleBsconfigChange(string)
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
            bsconfig: state.bsconfig,
            packages: state.packages,
            code: state.code,
          }
        | HandleBsconfigChange(data) => {
            bsconfig: data,
            packages: state.packages,
            result: state.result,
            code: state.code,
          }
        | HandlePackagesChange(data) => {
            packages: data,
            bsconfig: state.bsconfig,
            result: state.result,
            code: state.code,
          }
        | HandleCodeChange(data) => {
            code: data,
            bsconfig: state.bsconfig,
            packages: state.packages,
            result: state.result,
          }
        },
      {bsconfig: "", packages: "", code: "", result: ""},
    );

  let runCodeMutation =
    GraphQLQueries.SendCode.make(
      ~bsconfig=state.bsconfig,
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
             |> Option.fold(
                  "No Data yet. Wait till it loads, and please make sure to log the final output you want to see.",
                  d =>
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
                 placeholder="Enter list of bs-dependencies separated by spaces here"
                 className="form-control mt-2"
                 rows=5
                 value={state.bsconfig}
                 onChange={
                   event =>
                     dispatch(
                       HandleBsconfigChange(
                         ReactEvent.Form.target(event)##value,
                       ),
                     )
                 }
               />
               <textarea
                 placeholder="Enter list of packages separated by spaces here"
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
             <div> {data |> ReasonReact.string} </div>
           </div>;
         }
       }
  </GraphQLQueries.SendCodeMutation>;
};
