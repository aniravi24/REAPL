let onSubmit = event => {
  ReactEvent.Form.preventDefault(event);
  Js.log(event);
};

[@react.component]
let make = () =>
  <div className="container text-center font-weight-bold mt-2">
    {"REAPL" |> ReasonReact.string}
    <form className="form-group" onSubmit>
      <textarea
        placeholder="Enter list of packages separated by line here"
        className="form-control mt-2"
        rows=5
      />
      <textarea
        placeholder="Enter code here"
        className="form-control mt-5"
        rows=10
      />
      <button className="btn btn-primary mt-2">
        {"Submit" |> ReasonReact.string}
      </button>
    </form>
  </div>;
