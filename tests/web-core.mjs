import assert from 'node:assert/strict';import {readFileSync} from 'node:fs';import createCore from '../dist/core.js';
const c=await createCore(), get=c.cwrap('vb_get','number',['number']),action=c.cwrap('vb_action','number',['number','number']),load=c.cwrap('vb_load','number',['string']),save=c.cwrap('vb_save','string',[]);
assert.equal(get(15),0);assert.equal(action(0,0),1);assert.equal(action(5,0),1);c._vb_tick(1000);for(let t=1001;t<=1030;t++)c._vb_tick(t);assert.equal(get(30),30);assert.equal(get(9),129600);c._vb_tick(1031);assert.equal(get(7),1);action(7,1);c._vb_tick(1032);assert.equal(get(7),1);action(7,0);c._vb_tick(900000);assert.equal(get(7),1);action(6,0);c._vb_tick(900001);assert.equal(get(7),1);action(6,0);c._vb_tick(900002);assert.equal(get(7),2);
assert.equal(load(readFileSync('build/test-transfer.save','utf8')),1);assert.equal(get(16),1);assert.equal(get(22),5);assert.equal(get(0),4);const before=save();assert.equal(load('VIBE_BEASTS 99'),0);assert.equal(save(),before);const backup=save();assert.equal(load(backup),1);assert.equal(get(16),1);console.log('PASS: actual WebAssembly hatching, active clock, suspension, sleep/pause, native-to-web save and invalid import');

assert.equal(load(readFileSync('build/level-below.save','utf8')),1);
assert.equal(get(35),16);assert.equal(get(4),15);assert.equal(get(36),129600);
assert.equal(get(10),0);assert.equal(action(4,0),0);
assert.equal(load(readFileSync('build/time-below.save','utf8')),1);
assert.equal(get(4),16);assert.equal(get(36),129599);assert.equal(action(4,0),0);
c._vb_tick(1000);c._vb_tick(1001);
assert.equal(get(10),1);assert.equal(action(4,0),1);assert.equal(get(0),5);
assert.equal(get(35),36);assert.equal(get(9),259200);assert.equal(get(36),129600);
assert.equal(get(4),16);assert.equal(get(6),0);
assert.equal(load(readFileSync('build/evolution-ready.save','utf8')),1);
assert.equal(get(35),36);assert.equal(get(10),1);assert.equal(action(4,2),1);assert.equal(get(0),136);
assert.equal(get(36),259200);assert.equal(get(35),0);
console.log('PASS: actual WebAssembly level/time rejection, cumulative evolution clock, preserved level and converted Eevee branch');
