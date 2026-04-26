/**
 * Social Network Simulation — Main Application
 * 
 * Modules:
 * - SocialNetworkSim: Graph data structure (mirrors C++ adjacency list)
 * - GraphRenderer: Canvas 2D rendering engine
 * - BFSAnimator: Step-by-step BFS animation
 * 
 * TODO (Step 4): Implement GraphRenderer — draw nodes & edges
 * TODO (Step 5): Implement BFSAnimator — step-by-step animation
 * TODO (Step 6): Implement Friend Suggestion UI
 */

'use strict';

// ============ SOCIAL NETWORK (Graph Data Structure) ============

class SocialNetworkSim {
    constructor() {
        /** @type {Map<string, Set<string>>} Adjacency List */
        this.adjList = new Map();
    }

    addUser(name) {
        if (!this.adjList.has(name)) {
            this.adjList.set(name, new Set());
        }
    }

    addFriendship(u, v) {
        this.addUser(u);
        this.addUser(v);
        this.adjList.get(u).add(v);
        this.adjList.get(v).add(u);
    }

    removeFriendship(u, v) {
        if (this.adjList.has(u)) this.adjList.get(u).delete(v);
        if (this.adjList.has(v)) this.adjList.get(v).delete(u);
    }

    getFriends(user) {
        return this.adjList.has(user) ? [...this.adjList.get(user)] : [];
    }

    areFriends(u, v) {
        return this.adjList.has(u) && this.adjList.get(u).has(v);
    }

    getAllUsers() {
        return [...this.adjList.keys()];
    }

    getUserCount() {
        return this.adjList.size;
    }
}

// ============ GRAPH RENDERER (Canvas 2D) ============

class GraphRenderer {
    constructor(canvas) {
        this.canvas = canvas;
        this.ctx = canvas.getContext('2d');
        this.nodes = new Map();  // name -> {x, y, color, radius}
        this.edges = [];         // [{from, to, color}]
        this.resizeCanvas();
        window.addEventListener('resize', () => this.resizeCanvas());
    }

    resizeCanvas() {
        const container = this.canvas.parentElement;
        this.canvas.width = container.clientWidth;
        this.canvas.height = container.clientHeight;
    }

    // TODO (Step 4): Implement rendering methods

    render() {
        // Placeholder — will draw graph in Step 4
        const ctx = this.ctx;
        ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);
        ctx.fillStyle = '#9999b3';
        ctx.font = '16px Inter, sans-serif';
        ctx.textAlign = 'center';
        ctx.fillText('Graph will be rendered here (Step 4)', this.canvas.width / 2, this.canvas.height / 2);
    }
}

// ============ BFS ANIMATOR ============

class BFSAnimator {
    constructor(graph, renderer) {
        this.graph = graph;
        this.renderer = renderer;
        // TODO (Step 5): Implement BFS step-by-step animation
    }
}

// ============ APP INITIALIZATION ============

document.addEventListener('DOMContentLoaded', () => {
    const canvas = document.getElementById('graph-canvas');
    const network = new SocialNetworkSim();
    const renderer = new GraphRenderer(canvas);

    // Create sample network (10 users)
    const users = ['An', 'Binh', 'Cuong', 'Dung', 'Em', 'Phuc', 'Giang', 'Hoa', 'Khoa', 'Linh'];
    users.forEach(u => network.addUser(u));

    // Sample friendships
    network.addFriendship('An', 'Binh');
    network.addFriendship('An', 'Cuong');
    network.addFriendship('An', 'Dung');
    network.addFriendship('Binh', 'Cuong');
    network.addFriendship('Binh', 'Em');
    network.addFriendship('Cuong', 'Phuc');
    network.addFriendship('Dung', 'Giang');
    network.addFriendship('Em', 'Hoa');
    network.addFriendship('Phuc', 'Giang');
    network.addFriendship('Giang', 'Hoa');
    network.addFriendship('Hoa', 'Khoa');
    network.addFriendship('Khoa', 'Linh');

    // Populate sidebar user list
    const userListEl = document.getElementById('user-list');
    users.forEach(user => {
        const li = document.createElement('li');
        li.textContent = user;
        li.id = `user-${user}`;
        li.addEventListener('click', () => {
            document.querySelectorAll('#user-list li').forEach(el => el.classList.remove('active'));
            li.classList.add('active');
            showUserInfo(user);
        });
        userListEl.appendChild(li);
    });

    function showUserInfo(user) {
        const infoEl = document.getElementById('user-info');
        const friends = network.getFriends(user);
        infoEl.innerHTML = `
            <p><strong>${user}</strong></p>
            <p style="font-size:0.85rem;color:#9999b3;margin-top:4px">
                Friends (${friends.length}): ${friends.join(', ') || 'None'}
            </p>
        `;
    }

    // Initial render
    renderer.render();

    console.log('[Social Network Sim] Initialized with', network.getUserCount(), 'users');
});
