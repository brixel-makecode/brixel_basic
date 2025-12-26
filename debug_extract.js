const fs = require('fs');
console.log("Starting extraction...");
try {
    const mainPath = 'c:/makecode/main.ts';
    if (!fs.existsSync(mainPath)) {
        console.error("main.ts not found");
        process.exit(1);
    }
    const content = fs.readFileSync(mainPath, 'utf8');
    console.log("Read main.ts, length:", content.length);

    // Matches export const X = hex`...`; handling multiline
    const regex = /export const \w+_font = hex`[\s\S]*?`;/g;
    const matches = content.match(regex);

    if (!matches) {
        console.log("No matches found");
    } else {
        console.log("Found matches:", matches.length);
        const fontContent = "namespace Han_OLED_SSD1306 {\n" + matches.join("\n\n") + "\n}\n";
        fs.writeFileSync('c:/makecode/fonts.ts', fontContent);
        console.log("Wrote fonts.ts");

        const newMain = content.replace(regex, "");
        fs.writeFileSync('c:/makecode/main.ts', newMain);
        console.log("Updated main.ts");
    }

} catch (e) {
    console.error("Error:", e);
}
