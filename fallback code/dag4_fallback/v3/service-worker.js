// Cache name
var cacheName = 'tracker-pwa';

// Files to cache
var filesToCache = [
  '/',
  '/index.html',
  '/style.css',
  '/script.js',
  // Add other files you want to cache
];

// Install event
self.addEventListener('install', function(e) {
  e.waitUntil(
    caches.open(cacheName).then(function(cache) {
      return cache.addAll(filesToCache);
    })
  );
});

// Fetch event
self.addEventListener('fetch', function(e) {
  e.respondWith(
    caches.match(e.request).then(function(response) {
      return response || fetch(e.request);
    })
  );
});