const path = require('node:path');
const { copyFile, mkdir, readdir } = require('node:fs/promises');

const SRC_JSON_DIR = path.join(
  process.cwd(),
  'nitrogen',
  'generated',
  'shared',
  'json'
);

const DEST_JSON_DIR = path.join(
  process.cwd(),
  'lib',
  'nitrogen',
  'generated',
  'shared',
  'json'
);

async function copyJsonFiles() {
  try {
    await mkdir(DEST_JSON_DIR, { recursive: true });
    const files = await readdir(SRC_JSON_DIR);
    for (const file of files) {
      if (file.endsWith('.json')) {
        const src = path.join(SRC_JSON_DIR, file);
        const dest = path.join(DEST_JSON_DIR, file);
        await copyFile(src, dest);
        console.log(`Copied JSON: ${file}`);
      }
    }
  } catch (err) {
    console.warn('Failed to copy JSON view configs:', err.message);
  }
}

(async () => {
  try {
    await copyJsonFiles();
    console.log('All Nitrogen files patched successfully.');
  } catch (err) {
    console.error('Error while processing files:', err);
    process.exit(1);
  }
})();
