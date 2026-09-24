import assert from 'node:assert/strict';
import {readFileSync,readdirSync} from 'node:fs';
import {createHash} from 'node:crypto';
const manifest=JSON.parse(readFileSync('assets/sprites-manifest.json','utf8'));
assert.equal(manifest.selection,'yellow/gen1/56x56');
assert.equal(manifest.assets.length,151);
const expected=manifest.assets.map(a=>a.filename).sort();
assert.deepEqual(readdirSync('dist/assets').sort(),expected,'Only the approved Yellow sprites may be bundled');
for(const a of manifest.assets){
 assert.match(a.original_path,/^yellow\/gen1\/56x56\/(?:\d{3}|124-GB)\.png$/);
 assert.equal(a.source_url,`https://raw.githubusercontent.com/ShiraTheMogul/rby-sprites-project/${manifest.source_commit}/${a.original_path}`);
 assert.equal(a.filename,String(a.national_id).padStart(3,'0')+'.png');
 assert(a.national_id>=1&&a.national_id<=151);
 const bytes=readFileSync('dist/assets/'+a.filename);
 assert.equal(createHash('sha256').update(bytes).digest('hex'),a.sha256);
 assert.equal(bytes.readUInt32BE(16),56);assert.equal(bytes.readUInt32BE(20),56);
}
const ids=[...readFileSync('src/roster.inc','utf8').matchAll(/\{(\d+),"/g)].map(m=>+m[1]).sort((a,b)=>a-b);
assert.deepEqual(ids,manifest.assets.map(a=>a.national_id).sort((a,b)=>a-b));
console.log('PASS: all 151 game species use unchanged, approved Pokemon Yellow sprites; no beta, prototype or other sets');
