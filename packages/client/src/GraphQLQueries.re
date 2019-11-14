module SendCode = [%graphql
  {|
    mutation runCode($bsconfig: String!, $packages: String!, $code: String!) {
        runCode(bsconfig: $bsconfig, packages: $packages, code: $code) {
            result
        }
    }
|}
];

module SendCodeMutation = ReasonApollo.CreateMutation(SendCode);
