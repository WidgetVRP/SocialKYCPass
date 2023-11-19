import * as Kilt from "@kiltprotocol/sdk-js"
import axios from "axios"

async function main() {
    let api = await Kilt.connect('wss://spiritnet.kilt.io/')
    let apiConfig = Kilt.ConfigService.get('api')
    const encodedJohnDoeDetails =
      await apiConfig.call.did.queryByWeb3Name('john_doe')
    
    // This function will throw if johnDoeOwner does not exist
    const {
      document: { uri }
    } = Kilt.Did.linkedInfoFromChain(encodedJohnDoeDetails)
    console.log(`My name is widgetvrp and this is my DID: "${uri}"`)
    const johnDoeDidDocument = await Kilt.Did.resolve(uri)
    console.log(`John Doe's DID Document:`)
    console.log(JSON.stringify(johnDoeDidDocument, null, 2))
    
    const endpoints = johnDoeDidDocument?.document?.service
    if (!endpoints) {
      console.log('No endpoints for the DID.')
      return []
    }
    
    console.log('Endpoints:')
    console.log(JSON.stringify(endpoints, null, 2))

    const {
        data: [{ credential }]
      } = await axios.get<Kilt.KiltPublishedCredentialCollectionV1>(
        endpoints[0].serviceEndpoint[0]
      )
      console.log(`Credentials: ${JSON.stringify(credential, null, 2)}`)
      try {
        const { attester, revoked } =
          await Kilt.Credential.verifyCredential(credential)
      
        // Verify that the credential is not revoked. Exception caught by the catch {} block below.
        if (revoked) {
          throw new Error('The credential has been revoked, hence it is not valid.')
        }
        console.log(
          `John Doe's credential is valid and has been attested by ${attester}!`
        )
      } catch {
        console.log("John Doe's credential is not valid.")
      }
      await Kilt.disconnect()
    
}

main()