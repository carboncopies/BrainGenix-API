# RequestUsername forwarding

The API gateway injects `RequestUsername` into `/NES` and `/VSDA` RPC bodies before forwarding them upstream.

## Behavior

1. Client authenticates with `AuthKey` (query param or header).
2. Gateway verifies the JWT and reads the username.
3. For each RPC method object in the JSON array body (anything other than `ReqID`), the gateway sets `RequestUsername` to that username.
4. The modified body is forwarded to NES/VSDA.

Clients do not need to send `RequestUsername`. If present, the gateway overwrites it with the authenticated user.

## Example

Client request:

```json
[{"ReqID":0,"Simulation/Create":{"Name":"example","Seed":1}}]
```

Forwarded to NES:

```json
[{"ReqID":0,"Simulation/Create":{"Name":"example","Seed":1,"RequestUsername":"Admonishing"}}]
```

## Notes

- Not a new public route. Existing `/NES` and `/VSDA` endpoints are unchanged for clients.
- Used by NES to place filesystem outputs under `/var/lib/BrainGenix/NES/<username>/`. See the NES `Docs/SharedOutputStorage.md`.
