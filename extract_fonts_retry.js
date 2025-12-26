const fs = require('fs');
try {
    let c = fs.readFileSync('c:/makecode/main.backup.ts', 'utf8'); // Reading original logic if possible, or main.ts
    // If main.ts was already cleaned, we might have lost the data if the previous write failed?
    // Wait, I saw "Cleaned main.ts" output. This means main.ts IS cleaned.
    // If fonts.ts creation failed, I might have lost the data!
    // But I have the data in the chat history or I can regenerate it.
    // Actually, I can just use the hex strings I generated before.

    // Let's check if main.ts still has them or if I have a backup.
    // I see `main.backup.ts` in the file list earlier! 

    if (!fs.existsSync('c:/makecode/main.backup.ts')) {
        // Fallback: try reading main.ts (if it wasn't cleaned yet? No, the command said it was).
        // Actually, let's just re-generate the strings or read from the previous `main.ts` view.
        // I will assume main.backup.ts exists or I will fail.
        if (fs.existsSync('c:/makecode/main.ts')) {
            c = fs.readFileSync('c:/makecode/main.ts', 'utf8');
        }
    }

    const fonts = c.match(/export const [A-Z].+? = hex\`[\s\S]+?\`;/g);
    if (fonts) {
        let fontContent = 'namespace Han_OLED_SSD1306 {\n' + fonts.join('\n\n') + '\n}\n';
        fs.writeFileSync('c:/makecode/fonts.ts', fontContent);
        console.log('Extracted fonts to fonts.ts');
    } else {
        console.log('No fonts found to extract.');
    }
} catch (e) {
    console.error(e);
}
