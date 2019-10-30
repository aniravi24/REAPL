module SendCode = [%graphql
  {|
    mutation runCode($packages: String!, $code: String!) {
        runCode(packages: $packages, code: $code) {
            result
        }
    }
|}
];

module SendCodeMutation = ReasonApollo.CreateMutation(SendCode);
