import * as Kilt from '@kiltprotocol/sdk-js'
import { config as envConfig } from 'dotenv'
import {
  blake2AsU8a,
  keyExtractPath,
  keyFromPath,
  mnemonicGenerate,
  mnemonicToMiniSecret,
  sr25519PairFromSeed
} from '@polkadot/util-crypto'

// generateAccount
function generateAccount(
  mnemonic = Kilt.Utils.Crypto.mnemonicGenerate()
): {
  account: Kilt.KiltKeyringPair
  mnemonic: string
} {
  const keyring = new Kilt.Utils.Keyring({
    ss58Format: 38,
    type: 'sr25519'
  })
  return {
    account: keyring.addFromMnemonic(mnemonic) as Kilt.KiltKeyringPair,
    mnemonic
  }
}

function generateKeyAgreement(mnemonic: string) {
  const secretKeyPair = sr25519PairFromSeed(mnemonicToMiniSecret(mnemonic))
  const { path } = keyExtractPath('//did//keyAgreement//0')
  const { secretKey } = keyFromPath(secretKeyPair, path, 'sr25519')
  return Kilt.Utils.Crypto.makeEncryptionKeypairFromSeed(blake2AsU8a(secretKey))
}

// generateKeypairs
function generateKeypairs(mnemonic = mnemonicGenerate()) {
  const { account } = generateAccount(mnemonic)

  const authentication = {
    ...account.derive('//did//0'),
    type: 'sr25519'
  } as Kilt.KiltKeyringPair

  const keyAgreement = generateKeyAgreement(mnemonic)

  return {
    authentication: authentication,
    keyAgreement: keyAgreement
  }
}

// generateLightDid
function generateLightDid(mnemonic: string): Kilt.DidDocument {
  const { authentication, keyAgreement } = generateKeypairs(mnemonic)
  return Kilt.Did.createLightDidDocument({
    authentication: [authentication as Kilt.NewLightDidVerificationKey],
    keyAgreement: [keyAgreement]
  })
}

// createPresentation
async function createPresentation(
  credential: Kilt.ICredential,
  signCallback: Kilt.SignCallback,
  challenge?: string
): Promise<Kilt.ICredentialPresentation> {
  return Kilt.Credential.createPresentation({
    credential,
    signCallback,
    challenge
  })
}
// getChallenge
function getChallenge(): string {
  return Kilt.Utils.UUID.generate()
}

// verifyPresentation
async function verifyPresentation(
  presentation: Kilt.ICredentialPresentation,
  challenge: string,
  trustedAttesterUris: Kilt.DidUri[]
): Promise<boolean> {
  Kilt.ConfigService.get('api')

  try {
    const { revoked, attester } = await Kilt.Credential.verifyPresentation(
      presentation,
      { challenge }
    )

    if (revoked) {
      return false
    }
    return trustedAttesterUris.includes(attester)
  } catch {
    return false
  }
}

// verificationFlow
export async function verificationFlow(
  credential: Kilt.ICredential,
  signCallback: Kilt.SignCallback,
  trustedAttesterUris: Kilt.DidUri[] = []
) {
  const challenge = getChallenge()

  const presentation = await createPresentation(
    credential,
    signCallback,
    challenge
  )

  const isValid = await verifyPresentation(
    presentation,
    challenge,
    trustedAttesterUris
  )

  if (isValid) {
    console.log('Verification successful! You are allowed to enter the club 🎉')
  } else {
    console.log('Verification failed! 🚫')
  }
}

// Ana fonksiyon ve diğer işlemler.
if (require.main === module) {
  ;(async () => {
    envConfig()

    try {
      await Kilt.connect(process.env.WSS_ADDRESS as string)
      const claimerDidMnemonic = process.env.CLAIMER_DID_MNEMONIC as string
      const { authentication } = generateKeypairs(claimerDidMnemonic)
      const claimerDid = generateLightDid(claimerDidMnemonic)
      
      const attesterDid = process.env.ATTESTER_DID_URI as Kilt.DidUri
      // Load credential and claimer DID
      const credential = JSON.parse(process.env.CLAIMER_CREDENTIAL as string)
      await verificationFlow(
        credential,
        async ({ data }) => ({
          signature: authentication.sign(data),
          keyType: authentication.type,
          keyUri: `${claimerDid.uri}${claimerDid.authentication[0].id}`
        }),
        [attesterDid]
      )
    } catch (e) {
      console.log('Error in the verification flow')
      throw e
    }
  })()
}
