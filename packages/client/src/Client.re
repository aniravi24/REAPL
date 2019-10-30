/* in memory cache for caching GraphQL data */
let inMemoryCache = ApolloInMemoryCache.createInMemoryCache();

/* apollo link instance as a network interface for apollo client */
/* apollo link also has to be configured with headers */
/* we get token from local storage and configure apollo link headers with it */

/*
 let headers =
   switch (Util.getTokenFromStorage()) {
   | None => Json.Encode.object_([])
   | Some(token) =>
     Js.log(token);
     Json.Encode.object_([
       ("Authorization", Json.Encode.string("Bearer" ++ token)),
     ]);
   };
 */

let httpLink =
  ApolloLinks.createHttpLink(
    ~uri="http://localhost:8000/graphql",
    /*~headers,*/
    (),
  );

/* apollo client instance */
let instance =
  ReasonApollo.createApolloClient(~link=httpLink, ~cache=inMemoryCache, ());

[@bs.module] external gql: ReasonApolloTypes.gql = "graphql-tag";
