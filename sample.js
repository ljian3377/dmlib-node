var DmlibAddon = require("bindings")("dmlib-addon");
// Load the .env file if it exists
require("dotenv").config();

async function main() {
  // Enter your storage account name and shared key
  const accountName = process.env.ACCOUNT_NAME || "";
  const accountKey = process.env.ACCOUNT_KEY || "";
  const filePath = process.env.FILE_PATH || "";

  const begin = new Date();
  const ret = DmlibAddon.uploadFile(accountName, accountKey, filePath);
  const end = new Date();
  console.log(ret);
  console.log(`Time elapsed: ${end.getTime() - begin.getTime()} ms`);
}

main();
