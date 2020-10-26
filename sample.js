var DmlibAddon = require("bindings")("dmlib-addon");
// Load the .env file if it exists
require("dotenv").config();

async function delay(ms) {
  return new Promise((reslove, reject) => {
    setTimeout(reslove, ms);
  });
}

async function upload() {
  // Enter your storage account name and shared key
  const accountName = process.env.ACCOUNT_NAME || "";
  const accountSAS = process.env.ACCOUNT_SAS || "";
  const filePath = process.env.FILE_PATH || "";

  const begin = new Date();
  return new Promise((reslove, reject) => {
    DmlibAddon.uploadFile(
      filePath,
      accountName,
      accountSAS,
      async (status, msg) => {
        const end = new Date();
        console.log(`Time elapsed: ${end.getTime() - begin.getTime()} ms`);
        if (status === 0) {
          reslove();
        } else {
          reject(new Error(msg));
        }
      }
    );
  });
}

async function main() {
  await upload();
}

main().catch((err) => {
  console.error("Error running sample:", err.message);
});

setTimeout(() => {
  console.log("timeout");
}, 100 * 1000);
