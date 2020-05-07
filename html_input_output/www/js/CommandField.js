window.addEventListener("load", function() {
  document.getElementById('my-form').addEventListener("submit", function(e) {
    e.preventDefault(); // before the code
    /* do what you want with the form */

    // Should be triggered on form submit
    alert('hi');
  })
});
