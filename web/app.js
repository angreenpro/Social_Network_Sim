'use strict';

// ===== GRAPH DATA STRUCTURE (mirrors C++ adjacency list) =====
class SocialNetworkSim {
    constructor() { this.adjList = new Map(); }
    addUser(n) { if (!this.adjList.has(n)) this.adjList.set(n, new Set()); }
    addFriendship(u, v) { this.addUser(u); this.addUser(v); this.adjList.get(u).add(v); this.adjList.get(v).add(u); }
    getFriends(u) { return this.adjList.has(u) ? [...this.adjList.get(u)] : []; }
    areFriends(u, v) { return this.adjList.has(u) && this.adjList.get(u).has(v); }
    getAllUsers() { return [...this.adjList.keys()]; }
    getUserCount() { return this.adjList.size; }
    clear() { this.adjList.clear(); }

    // BFS traversal — yields steps for animation
    *bfsSteps(start) {
        if (!this.adjList.has(start)) return;
        const visited = new Set([start]);
        const queue = [start];
        yield { type: 'init', node: start, queue: [...queue], visited: new Set(visited) };
        while (queue.length > 0) {
            const current = queue.shift();
            yield { type: 'visit', node: current, queue: [...queue], visited: new Set(visited) };
            const friends = this.getFriends(current).sort();
            for (const nb of friends) {
                if (!visited.has(nb)) {
                    visited.add(nb); queue.push(nb);
                    yield { type: 'discover', node: nb, from: current, queue: [...queue], visited: new Set(visited) };
                }
            }
        }
        yield { type: 'done', visited: new Set(visited) };
    }

    // Friend suggestion — BFS depth 2
    suggestFriends(user) {
        if (!this.adjList.has(user)) return [];
        const friends = this.adjList.get(user);
        const candidates = new Map();
        for (const f of friends) {
            for (const fof of this.getFriends(f)) {
                if (fof !== user && !friends.has(fof)) {
                    candidates.set(fof, (candidates.get(fof) || 0) + 1);
                }
            }
        }
        return [...candidates.entries()].sort((a, b) => b[1] - a[1] || a[0].localeCompare(b[0]));
    }
}

// ===== FILE PARSER =====
function parseNetworkFile(text) {
    const lines = text.split(/\r?\n/);
    let section = '';
    const users = [], edges = [];
    for (const raw of lines) {
        const line = raw.trim();
        if (!line || line.startsWith('#')) continue;
        if (line === 'USERS') { section = 'USERS'; continue; }
        if (line === 'FRIENDSHIPS') { section = 'FRIENDSHIPS'; continue; }
        if (section === 'USERS') users.push(line);
        else if (section === 'FRIENDSHIPS') {
            const parts = line.split(/\s+/);
            if (parts.length >= 2) edges.push([parts[0], parts[1]]);
        }
    }
    return { users, edges };
}

// ===== COLORS =====
const COLORS = {
    unvisited: '#4a4a6a', inQueue: '#f5a623', visiting: '#22d3a0',
    visited: '#4f8cff', suggested: '#a855f7',
    edgeDefault: 'rgba(255,255,255,0.07)', edgeHighlight: 'rgba(79,140,255,0.45)',
    edgeSuggested: 'rgba(168,85,247,0.4)',
    nodeBorder: 'rgba(255,255,255,0.15)', textColor: '#e8e8f0',
};

// ===== GRAPH RENDERER (Canvas 2D + Force-Directed Layout) =====
class GraphRenderer {
    constructor(canvas) {
        this.canvas = canvas;
        this.ctx = canvas.getContext('2d');
        this.nodes = new Map(); // name -> {x, y, vx, vy, radius, state}
        this.edges = []; // [{from, to, state}]
        this.selectedNode = null;
        this.hoveredNode = null;
        this.dragNode = null;
        this.mouse = { x: 0, y: 0 };
        this.nodeRadius = 22;
        this.running = false;
        this._onSelect = null;
        this._resizeCanvas();
        window.addEventListener('resize', () => { this._resizeCanvas(); this.render(); });
        this._initMouse();
    }

    _resizeCanvas() {
        const c = this.canvas.parentElement;
        this.canvas.width = c.clientWidth * devicePixelRatio;
        this.canvas.height = c.clientHeight * devicePixelRatio;
        this.canvas.style.width = c.clientWidth + 'px';
        this.canvas.style.height = c.clientHeight + 'px';
        this.ctx.setTransform(devicePixelRatio, 0, 0, devicePixelRatio, 0, 0);
        this.W = c.clientWidth;
        this.H = c.clientHeight;
    }

    _initMouse() {
        const c = this.canvas;
        c.addEventListener('mousedown', e => {
            const n = this._hitTest(e.offsetX, e.offsetY);
            if (n) { this.dragNode = n; c.classList.add('dragging'); }
        });
        c.addEventListener('mousemove', e => {
            this.mouse.x = e.offsetX; this.mouse.y = e.offsetY;
            if (this.dragNode) {
                const nd = this.nodes.get(this.dragNode);
                nd.x = e.offsetX; nd.y = e.offsetY; nd.vx = 0; nd.vy = 0;
                this.render();
            } else {
                const prev = this.hoveredNode;
                this.hoveredNode = this._hitTest(e.offsetX, e.offsetY);
                if (prev !== this.hoveredNode) this.render();
                this._updateTooltip();
            }
        });
        c.addEventListener('mouseup', () => {
            if (this.dragNode) { c.classList.remove('dragging'); this.dragNode = null; }
        });
        c.addEventListener('mouseleave', () => {
            this.hoveredNode = null; this.dragNode = null;
            c.classList.remove('dragging');
            this._hideTooltip(); this.render();
        });
        c.addEventListener('click', e => {
            const n = this._hitTest(e.offsetX, e.offsetY);
            if (n) { this.selectedNode = n; if (this._onSelect) this._onSelect(n); this.render(); }
        });
    }

    _hitTest(mx, my) {
        for (const [name, nd] of this.nodes) {
            const dx = mx - nd.x, dy = my - nd.y;
            if (dx * dx + dy * dy <= (nd.radius + 4) ** 2) return name;
        }
        return null;
    }

    _updateTooltip() {
        const tip = document.getElementById('tooltip');
        if (this.hoveredNode && !this.dragNode) {
            const nd = this.nodes.get(this.hoveredNode);
            tip.textContent = this.hoveredNode;
            tip.style.left = (nd.x + 30) + 'px';
            tip.style.top = (nd.y - 10) + 'px';
            tip.classList.remove('hidden');
        } else this._hideTooltip();
    }
    _hideTooltip() { document.getElementById('tooltip').classList.add('hidden'); }

    onSelect(cb) { this._onSelect = cb; }

    // Build nodes/edges from network
    buildGraph(network) {
        this.nodes.clear(); this.edges = [];
        const users = network.getAllUsers();
        const cx = this.W / 2, cy = this.H / 2;
        const r = Math.min(this.W, this.H) * 0.32;
        users.forEach((u, i) => {
            const angle = (2 * Math.PI * i) / users.length - Math.PI / 2;
            this.nodes.set(u, {
                x: cx + r * Math.cos(angle) + (Math.random() - 0.5) * 20,
                y: cy + r * Math.sin(angle) + (Math.random() - 0.5) * 20,
                vx: 0, vy: 0, radius: this.nodeRadius, state: 'unvisited'
            });
        });
        const seen = new Set();
        for (const u of users) {
            for (const v of network.getFriends(u)) {
                const key = [u, v].sort().join('-');
                if (!seen.has(key)) { seen.add(key); this.edges.push({ from: u, to: v, state: 'default' }); }
            }
        }
        this._runForceLayout();
    }

    // Force-directed layout (simple spring-charge model)
    _runForceLayout() {
        this.running = true;
        let iterations = 0;
        const maxIter = 200;
        const step = () => {
            if (iterations >= maxIter || !this.running) { this.running = false; this.render(); return; }
            this._forceStep();
            this.render();
            iterations++;
            requestAnimationFrame(step);
        };
        requestAnimationFrame(step);
    }

    _forceStep() {
        const nodes = [...this.nodes.entries()];
        const k = 120; // ideal spring length
        const repulsion = 8000;
        const damping = 0.85;
        const pad = 40;
        // Repulsion between all pairs
        for (let i = 0; i < nodes.length; i++) {
            for (let j = i + 1; j < nodes.length; j++) {
                const [, a] = nodes[i], [, b] = nodes[j];
                let dx = b.x - a.x, dy = b.y - a.y;
                let dist = Math.sqrt(dx * dx + dy * dy) || 1;
                const force = repulsion / (dist * dist);
                const fx = (dx / dist) * force, fy = (dy / dist) * force;
                a.vx -= fx; a.vy -= fy; b.vx += fx; b.vy += fy;
            }
        }
        // Spring attraction along edges
        for (const e of this.edges) {
            const a = this.nodes.get(e.from), b = this.nodes.get(e.to);
            let dx = b.x - a.x, dy = b.y - a.y;
            let dist = Math.sqrt(dx * dx + dy * dy) || 1;
            const force = (dist - k) * 0.04;
            const fx = (dx / dist) * force, fy = (dy / dist) * force;
            a.vx += fx; a.vy += fy; b.vx -= fx; b.vy -= fy;
        }
        // Center gravity
        const cx = this.W / 2, cy = this.H / 2;
        for (const [, nd] of this.nodes) {
            nd.vx += (cx - nd.x) * 0.002; nd.vy += (cy - nd.y) * 0.002;
            nd.vx *= damping; nd.vy *= damping;
            if (this.dragNode && nd === this.nodes.get(this.dragNode)) continue;
            nd.x += nd.vx; nd.y += nd.vy;
            nd.x = Math.max(pad, Math.min(this.W - pad, nd.x));
            nd.y = Math.max(pad, Math.min(this.H - pad, nd.y));
        }
    }

    setNodeState(name, state) { if (this.nodes.has(name)) this.nodes.get(name).state = state; }
    setEdgeState(from, to, state) {
        const e = this.edges.find(e => (e.from === from && e.to === to) || (e.from === to && e.to === from));
        if (e) e.state = state;
    }
    resetStates() {
        for (const [, n] of this.nodes) n.state = 'unvisited';
        for (const e of this.edges) e.state = 'default';
    }

    // ===== RENDER =====
    render() {
        const ctx = this.ctx;
        ctx.clearRect(0, 0, this.W, this.H);
        // Draw edges
        for (const e of this.edges) {
            const a = this.nodes.get(e.from), b = this.nodes.get(e.to);
            if (!a || !b) continue;
            ctx.beginPath(); ctx.moveTo(a.x, a.y); ctx.lineTo(b.x, b.y);
            ctx.strokeStyle = e.state === 'highlight' ? COLORS.edgeHighlight
                            : e.state === 'suggested' ? COLORS.edgeSuggested
                            : COLORS.edgeDefault;
            ctx.lineWidth = e.state === 'default' ? 1.5 : 2.5;
            ctx.stroke();
        }
        // Draw nodes
        for (const [name, nd] of this.nodes) {
            const color = COLORS[nd.state] || COLORS.unvisited;
            const isHovered = name === this.hoveredNode;
            const isSelected = name === this.selectedNode;
            const r = nd.radius + (isHovered ? 3 : 0);
            // Glow
            if (nd.state !== 'unvisited' || isSelected) {
                ctx.beginPath(); ctx.arc(nd.x, nd.y, r + 8, 0, Math.PI * 2);
                const glow = ctx.createRadialGradient(nd.x, nd.y, r, nd.x, nd.y, r + 12);
                glow.addColorStop(0, color + '40'); glow.addColorStop(1, 'transparent');
                ctx.fillStyle = glow; ctx.fill();
            }
            // Circle
            ctx.beginPath(); ctx.arc(nd.x, nd.y, r, 0, Math.PI * 2);
            const grad = ctx.createRadialGradient(nd.x, nd.y - r * 0.3, r * 0.1, nd.x, nd.y, r);
            grad.addColorStop(0, color + 'dd'); grad.addColorStop(1, color + '88');
            ctx.fillStyle = grad; ctx.fill();
            ctx.strokeStyle = isSelected ? '#ffffff' : COLORS.nodeBorder;
            ctx.lineWidth = isSelected ? 2.5 : 1;
            ctx.stroke();
            // Label
            ctx.fillStyle = COLORS.textColor;
            ctx.font = `${isSelected ? '600' : '500'} ${isHovered ? 13 : 12}px Inter, sans-serif`;
            ctx.textAlign = 'center'; ctx.textBaseline = 'middle';
            ctx.fillText(name, nd.x, nd.y);
        }
    }
}

// ===== APP CONTROLLER =====
document.addEventListener('DOMContentLoaded', () => {
    const canvas = document.getElementById('graph-canvas');
    const network = new SocialNetworkSim();
    const renderer = new GraphRenderer(canvas);

    // DOM refs
    const userListEl = document.getElementById('user-list');
    const userCountBadge = document.getElementById('user-count-badge');
    const userInfoEl = document.getElementById('user-info');
    const suggestionsEl = document.getElementById('suggestions-list');
    const dataStatus = document.getElementById('data-status');
    const fileInput = document.getElementById('file-input');
    const btnPlay = document.getElementById('btn-bfs-play');
    const btnStep = document.getElementById('btn-bfs-step');
    const btnPause = document.getElementById('btn-bfs-pause');
    const btnReset = document.getElementById('btn-bfs-reset');
    const btnSuggest = document.getElementById('btn-suggest');
    const speedSlider = document.getElementById('speed-slider');
    const infoMode = document.getElementById('bfs-mode');
    const infoQueue = document.getElementById('bfs-queue');
    const infoVisited = document.getElementById('bfs-visited');
    const infoStep = document.getElementById('bfs-step-count');

    // BFS state
    let bfsIterator = null;
    let bfsPlaying = false;
    let bfsTimer = null;
    let stepCount = 0;

    // ---- Load network data ----
    function loadNetworkData(text) {
        const { users, edges } = parseNetworkFile(text);
        network.clear();
        users.forEach(u => network.addUser(u));
        edges.forEach(([u, v]) => network.addFriendship(u, v));
        renderer.buildGraph(network);
        populateUserList();
        dataStatus.textContent = `${network.getUserCount()} users, ${edges.length} edges`;
        dataStatus.className = 'loaded';
        userCountBadge.textContent = network.getUserCount();
        btnPlay.disabled = true; btnStep.disabled = true; btnSuggest.disabled = false;
        resetBFS();
        userInfoEl.innerHTML = '<p class="placeholder">Click a node to view info</p>';
        suggestionsEl.innerHTML = '<p class="placeholder">Select a user first</p>';
    }

    // Try auto-loading from data folder (works when served from project root)
    const dataUrls = ['../data/network.txt', '/data/network.txt', 'data/network.txt'];
    (async () => {
        for (const url of dataUrls) {
            try {
                const r = await fetch(url);
                if (r.ok) { loadNetworkData(await r.text()); return; }
            } catch {}
        }
        dataStatus.textContent = 'Click button to load file';
    })();

    // File picker
    fileInput.addEventListener('change', e => {
        const file = e.target.files[0];
        if (!file) return;
        const reader = new FileReader();
        reader.onload = ev => loadNetworkData(ev.target.result);
        reader.readAsText(file);
    });
    document.getElementById('load-file-btn').addEventListener('click', () => fileInput.click());

    // ---- Sidebar user list ----
    function populateUserList() {
        userListEl.innerHTML = '';
        network.getAllUsers().sort().forEach(user => {
            const li = document.createElement('li');
            li.textContent = user;
            li.id = `user-${user}`;
            li.addEventListener('click', () => selectUser(user));
            userListEl.appendChild(li);
        });
    }

    function selectUser(user) {
        document.querySelectorAll('#user-list li').forEach(el => el.classList.remove('active'));
        const el = document.getElementById(`user-${user}`);
        if (el) el.classList.add('active');
        renderer.selectedNode = user;
        renderer.render();
        showUserInfo(user);
        showSuggestions(user);
        btnPlay.disabled = false;
        btnStep.disabled = false;
        resetBFS();
    }

    renderer.onSelect(user => selectUser(user));

    function showUserInfo(user) {
        const friends = network.getFriends(user).sort();
        userInfoEl.innerHTML = `
            <p class="info-name">${user}</p>
            <p class="info-friends">Friends (${friends.length}):<br>
                ${friends.map(f => `<span>${f}</span>`).join(' ') || '<em>None</em>'}
            </p>`;
    }

    function showSuggestions(user) {
        const suggestions = network.suggestFriends(user);
        if (suggestions.length === 0) {
            suggestionsEl.innerHTML = '<p class="placeholder">No suggestions</p>';
            return;
        }
        suggestionsEl.innerHTML = suggestions.map(([name, mutual]) => `
            <div class="suggestion-item" data-user="${name}">
                <span class="name">${name}</span>
                <span class="mutual">${mutual} mutual</span>
            </div>`).join('');
        suggestionsEl.querySelectorAll('.suggestion-item').forEach(el => {
            el.addEventListener('click', () => {
                renderer.resetStates();
                renderer.setNodeState(el.dataset.user, 'suggested');
                renderer.render();
            });
        });
    }

    // ---- BFS Animation ----
    function resetBFS() {
        bfsPlaying = false;
        if (bfsTimer) { clearTimeout(bfsTimer); bfsTimer = null; }
        bfsIterator = null;
        stepCount = 0;
        renderer.resetStates();
        renderer.render();
        updateBFSInfo('Idle', [], new Set(), 0);
        btnPause.disabled = true;
    }

    function updateBFSInfo(mode, queue, visited, step) {
        infoMode.textContent = mode;
        infoQueue.textContent = queue.length ? queue.join(', ') : '—';
        infoQueue.title = queue.join(', ');
        infoVisited.textContent = visited.size ? [...visited].join(', ') : '—';
        infoVisited.title = [...visited].join(', ');
        infoStep.textContent = step;
    }

    function doBFSStep() {
        if (!bfsIterator) {
            const start = renderer.selectedNode;
            if (!start) return false;
            renderer.resetStates();
            bfsIterator = network.bfsSteps(start);
            stepCount = 0;
        }
        const { value, done } = bfsIterator.next();
        if (done || !value) {
            bfsPlaying = false;
            btnPause.disabled = true;
            updateBFSInfo('Done', [], value?.visited || new Set(), stepCount);
            return false;
        }
        stepCount++;
        const s = value;
        if (s.type === 'init') {
            renderer.setNodeState(s.node, 'inQueue');
            updateBFSInfo('BFS', s.queue, s.visited, stepCount);
        } else if (s.type === 'visit') {
            renderer.setNodeState(s.node, 'visiting');
            updateBFSInfo('Visiting: ' + s.node, s.queue, s.visited, stepCount);
            // After a brief moment mark as visited (visual effect)
            setTimeout(() => {
                renderer.setNodeState(s.node, 'visited');
                renderer.render();
            }, Math.min(parseInt(speedSlider.value) * 0.4, 300));
        } else if (s.type === 'discover') {
            renderer.setNodeState(s.node, 'inQueue');
            renderer.setEdgeState(s.from, s.node, 'highlight');
            updateBFSInfo('Discover: ' + s.node, s.queue, s.visited, stepCount);
        } else if (s.type === 'done') {
            updateBFSInfo('Done', [], s.visited, stepCount);
            bfsPlaying = false;
            btnPause.disabled = true;
            return false;
        }
        renderer.render();
        return true;
    }

    function playBFS() {
        bfsPlaying = true;
        btnPause.disabled = false;
        const tick = () => {
            if (!bfsPlaying) return;
            const cont = doBFSStep();
            if (cont) bfsTimer = setTimeout(tick, parseInt(speedSlider.value));
        };
        tick();
    }

    btnPlay.addEventListener('click', () => {
        if (!renderer.selectedNode) return;
        playBFS();
    });
    btnStep.addEventListener('click', () => {
        if (!renderer.selectedNode) return;
        bfsPlaying = false;
        doBFSStep();
    });
    btnPause.addEventListener('click', () => {
        bfsPlaying = false;
        if (bfsTimer) { clearTimeout(bfsTimer); bfsTimer = null; }
        btnPause.disabled = true;
    });
    btnReset.addEventListener('click', () => resetBFS());

    // ---- Friend Suggestion Visualization ----
    btnSuggest.addEventListener('click', () => {
        const user = renderer.selectedNode;
        if (!user) return;
        resetBFS();
        renderer.resetStates();
        // Highlight selected user
        renderer.setNodeState(user, 'visiting');
        // Highlight direct friends (depth 1)
        const friends = network.getFriends(user);
        friends.forEach(f => {
            renderer.setNodeState(f, 'visited');
            renderer.setEdgeState(user, f, 'highlight');
        });
        // Highlight suggestions (depth 2)
        const suggestions = network.suggestFriends(user);
        suggestions.forEach(([name]) => {
            renderer.setNodeState(name, 'suggested');
            // highlight edges from mutual friends
            friends.forEach(f => {
                if (network.areFriends(f, name)) {
                    renderer.setEdgeState(f, name, 'suggested');
                }
            });
        });
        renderer.render();
        updateBFSInfo('Suggest', [], new Set([user, ...friends, ...suggestions.map(s => s[0])]), 0);
        showSuggestions(user);
    });

    console.log('[Social Network Sim] Ready');
});
