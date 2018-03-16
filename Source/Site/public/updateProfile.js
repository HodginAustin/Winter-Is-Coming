function updateProfile(id){
  $.ajax({
    url: '/profiles/' + id,
    type: 'put',
    data: $('update-profile').serialize(),
    success: function(results){
      window.location.replace("./");
    }
  });
}
