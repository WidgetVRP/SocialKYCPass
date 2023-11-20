# SocialKYCPass

# Project Report: RFID Mifare Classic 4K Card Data Storage and Management

![alternative text](logo.png "Image Title")

## Overview

This project aims to implement a comprehensive data storage and management system on a Mifare Classic 4K RFID card. The card, with a total storage capacity of 4K, is segmented into specific parts called slots, each designed to hold distinct types of data. This structure allows for efficient organization and retrieval of data stored on the card.

## Card Structure and Capacity

The Mifare Classic 4K card consists of 40 sectors, with the first 32 sectors comprising 16 bytes each and the remaining sectors (33 to 39) containing 64 bytes each. Due to the presence of control bytes in each sector, the last element of each sector is unusable for data storage, leaving approximately 3440 bytes available for use.

## Slot Configuration

1. **Information Slot (Slot 1 - Sector 0):** 
   - Comprises 4 blocks, with the first being the UID block and the last being the control block.
   - Stores metadata about the data in other slots.
   - The second and third blocks contain specific information about the data stored in Veri Slotu1 and Veri Slotu2.

2. **Data Slot 1 (Veri Slotu1):**
   - Spans from Sector 1 Block 0 to a maximum of Sector 31 Block 3.
   - Designed to store the first type of data.

3. **Data Slot 2 (Veri Slotu2):**
   - Begins at Sector 33 Block 0 and extends up to Sector 39 Block 14.
   - Stores the second type of data.

4. **Security Slot:**
   - Located in Sector 32.
   - Initially planned to store 32 bytes of data.
   - Positioned between the two data slots for optimal security management.

## Information Slot Details

- **Sector 0 Block 1:**
  - The first 4 bytes indicate the type of document stored in Veri Slotu1 (ranging from 0 to 10).
  - The next 4 bytes denote the starting slot and block for Veri Slotu1's data in hexadecimal format.
  - The following 4 bytes specify the sector and block where Veri Slotu1's data concludes.

- **Sector 0 Block 2:**
  - Structured similarly to Block 1 but contains information for Veri Slotu2.

## Example Scenario

- If Veri Slotu1 contains a document of type 8 starting from Sector 1, Block 0, and ending in Sector 31, Block 3, Sector 0 Block 1 would read: `0800 0100 1F03 0000`.
- If Veri Slotu2 holds a document of type 20 beginning in Sector 33, Block 0, and finishing in Sector 39, Block 14, Sector 0 Block 2 would display: `1400 2100 270E 0000`.

## Special Considerations

- The last block of every sector, being a control byte, cannot be used for data storage.
- The Security Slot, positioned strategically between the two data slots, plays a crucial role in the overall security and integrity of the data.

## Conclusion

This project outlines a structured approach to utilizing the storage capabilities of the Mifare Classic 4K RFID card. Through careful planning and sector/block allocation, data can be efficiently stored and managed, with emphasis on accessibility and security. The designated slots for different data types and a dedicated security slot ensure both organizational clarity and data protection.
